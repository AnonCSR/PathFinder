#include "tensor_store.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <numeric>
#include <stdexcept>

#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include "storage/index/tensor_store/lsh/forest_index.h"
#include "storage/index/tensor_store/lsh/forest_query_iter.h"
#include "storage/index/tensor_store/lsh/tree.h"
#include "storage/index/tensor_store/serialization.h"
#include "storage/index/tensor_store/tensor_buffer_manager.h"

TensorStore::TensorStore(const std::string& name_, uint64_t tensors_dim_) :
    name            (name_),
    tensors_dim     (tensors_dim_),
    tensors_file_id (FileId::UNASSIGNED),
    mapping_path    (file_manager.get_file_path(name_ + ".mapping")),
    index_path      (file_manager.get_file_path(name_ + ".index"))
{
    if (tensors_dim < 1)
        throw std::invalid_argument("Tensor dimension must be at least 1");

    // Create a new tensors file
    std::string tensors_path = file_manager.get_file_path(name + ".tensors");
    if (Filesystem::is_regular_file(tensors_path))
        std::remove(tensors_path.c_str());
    tensors_file_id = file_manager.get_file_id(name + ".tensors");

    // Create a new mapping file with the initial header
    std::fstream ofs;
    ofs.open(mapping_path, std::ios::out | std::ios::binary | std::ios::trunc);
    Serialization::write_bool(ofs, false);
    Serialization::write_uint64(ofs, tensors_dim);
    Serialization::write_uint64(ofs, 0ULL);
    ofs.close();

    set_tensor_buffer_manager();
}


TensorStore::TensorStore(const std::string& name_) :
    name            (name_),
    tensors_file_id (FileId::UNASSIGNED),
    mapping_path    (file_manager.get_file_path(name_ + ".mapping")),
    index_path      (file_manager.get_file_path(name_ + ".index"))
{
    if (!exists(name_))
        throw std::invalid_argument("Tensor store " + name_ + " does not exist!");

    // Load tensors file
    tensors_file_id = file_manager.get_file_id(name + ".tensors");

    // Load object_id2tensor_offset and forest_index (if it exists) from disk
    deserialize();
    assert(tensors_dim > 0);

    set_tensor_buffer_manager();
}


TensorStore::~TensorStore() {
    // Save object_id2tensor_offset and forest_index to disk
    serialize();
}


bool TensorStore::exists(const std::string& name) {
    if (!Filesystem::is_regular_file(file_manager.get_file_path(name + ".tensors")))
        return false;
    else if (!Filesystem::is_regular_file(file_manager.get_file_path(name + ".mapping")))
        return false;
    return true;
}



bool TensorStore::contains(uint64_t object_id) const {
    return object_id2tensor_offset.find(object_id) != object_id2tensor_offset.end();
}


std::vector<float> TensorStore::get(uint64_t object_id) const {
    auto it = object_id2tensor_offset.find(object_id);
    if (it == object_id2tensor_offset.end()) {
        throw std::out_of_range("Object id " + std::to_string(object_id) + " not found");
    }
    // Compute tensor position
    auto  page_size   = tensor_buffer_manager->get_page_size();
    auto  page_number = it->second / page_size;
    auto  page_offset = it->second % page_size;
    auto& page        = tensor_buffer_manager->get_page(page_number);
    auto* ptr         = page.get_bytes() + page_offset;
    // Read tensor
    std::vector<float> tensor;
    tensor.resize(tensors_dim);
    std::memcpy(tensor.data(), ptr, sizeof(float) * tensor.size());
    tensor_buffer_manager->unpin(page);
    return tensor;
}


void TensorStore::insert(uint64_t object_id, const std::vector<float>& tensor) {
    assert(tensor.size() == tensors_dim);
    uint64_t tensor_offset;
    auto     it = object_id2tensor_offset.find(object_id);
    if (it != object_id2tensor_offset.end()) {
        // Replace an existing tensor
        tensor_offset = it->second;
    } else {
        // Insert a new tensor
        tensor_offset = object_id2tensor_offset.size() * sizeof(float) * tensors_dim;
        object_id2tensor_offset.insert({ object_id, tensor_offset });
    }
    // Compute tensor position
    auto  page_size   = tensor_buffer_manager->get_page_size();
    auto  page_number = tensor_offset / page_size;
    auto  page_offset = tensor_offset % page_size;
    auto& page        = tensor_buffer_manager->get_page(page_number);
    auto* ptr         = page.get_bytes() + page_offset;
    std::memcpy(ptr, tensor.data(), sizeof(float) * tensor.size());
    page.make_dirty();
    tensor_buffer_manager->unpin(page);
}


size_t TensorStore::size() const {
    return object_id2tensor_offset.size();
}


void TensorStore::build_forest_index(uint64_t num_trees, uint64_t max_depth) {
    forest_index = std::make_unique<LSH::ForestIndex>(num_trees, max_depth, *this);
    for (auto&& [object_id, tensor_offset] : object_id2tensor_offset)
        forest_index->insert(object_id);
}


std::unique_ptr<LSH::ForestQueryIter>
  TensorStore::get_forest_query_iter(const std::vector<float>& query_tensor, const LSH::Metric& metric) const {
    if (forest_index == nullptr)
        throw std::logic_error("Forest index is not built yet!");
    return forest_index->get_forest_query_iter(query_tensor, metric);
}


std::vector<std::pair<uint64_t, float>> 
  TensorStore::query_top_k(const std::vector<float>& query_tensor, const LSH::Metric& metric, uint64_t k) const {
    if (forest_index == nullptr)
        throw std::logic_error("Forest index is not built yet!");
    return forest_index->query_top_k(query_tensor, metric, k);
}


void TensorStore::serialize() const {
    bool has_forest_index = forest_index != nullptr;
    // Serialize mapping
    std::fstream ofs(mapping_path, std::ios::out | std::ios::binary | std::ios::trunc);
    Serialization::write_bool(ofs, has_forest_index);
    Serialization::write_uint64(ofs, tensors_dim);
    Serialization::write_uint64(ofs, object_id2tensor_offset.size());
    Serialization::write_uint642uint64_unordered_map(ofs, object_id2tensor_offset);
    ofs.close();

    // Serialize forest index
    if (forest_index != nullptr)
        forest_index->serialize(index_path);
}


void TensorStore::deserialize() {
    // Deserialize mapping
    std::fstream ifs(mapping_path, std::ios::in | std::ios::binary);
    auto         has_forest_index     = Serialization::read_bool(ifs);
    tensors_dim                       = Serialization::read_uint64(ifs);
    auto object_id2tensor_offset_size = Serialization::read_uint64(ifs);
    object_id2tensor_offset = Serialization::read_uint642uint64_unordered_map(ifs, object_id2tensor_offset_size);
    ifs.close();

    // Deserialize forest index
    if (has_forest_index) {
        assert(Filesystem::is_regular_file(index_path));
        forest_index = std::make_unique<LSH::ForestIndex>(index_path, *this);
    }
}


void TensorStore::set_tensor_buffer_manager() {
    // Compute the smallest number of pages of 4KB that can fit the tensors aligned
    uint_fast32_t N = 1024 / std::gcd(tensors_dim, 1024);
    // Compute the size in bytes of the TensorBufferManager page
    uint_fast32_t page_size = N * tensors_dim * sizeof(float);
    // Compute the number of pages to have at least TensorBufferManager::DEFAULT_BUFFER_SIZE bytes in total
    uint_fast32_t num_pages = 1 + ((TensorBufferManager::DEFAULT_BUFFER_SIZE - 1) / page_size);
    tensor_buffer_manager = std::make_unique<TensorBufferManager>(tensors_file_id, num_pages, page_size);
}
