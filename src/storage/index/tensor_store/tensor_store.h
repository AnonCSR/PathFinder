#pragma once
/*
 * TensorStore is an on-disk map between object ids and float tensors. The supported operations are insert (also
 * with replacement) and get. It has its own buffer manager for each instance.
 * The TensorStore disk storage are the .tensors and .mapping files:
 *
 * {tensor_store_name}.tensors - stores all the tensor data as an adjacently-arranged vector of floats
 *
 * {tensor_store_name}.mapping - stores the header (bool has_forest_index and uint64 tensors_dim) and the mapping
 * between object_id2tensor_offset map for being loaded into memory
 *
 * {tensor_store_name}.index   - stores the forest index serialized if it was previously built
 */

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "storage/file_id.h"

class TensorBufferManager;

namespace LSH {
class ForestIndex;
class Metric;
class ForestQueryIter;
} // namespace LSH

class TensorStore {
public:
    const std::string name;
    uint64_t          tensors_dim;


    // Check if a tensor store with a given name exists. At least the mapping and tensor files must exist
    static bool exists(const std::string& name);

    // Initialize a new forest index
    TensorStore(const std::string& name, uint64_t tensors_dim);

    // Load an existing tensor store with a given name
    TensorStore(const std::string& name);

    // Trigger the serialization before being destroyed
    ~TensorStore();

    // Check if a tensor with the given id exists
    bool contains(uint64_t object_id) const;

    // Get a tensor with the given object id
    std::vector<float> get(uint64_t object_id) const;

    // Insert a new tensor or replace an existing one
    void insert(uint64_t object_id, const std::vector<float>& tensor);

    size_t size() const;

    // Build and set a forest index with the entire tensor store
    void build_forest_index(uint64_t num_trees, uint64_t max_depth);

    std::unique_ptr<LSH::ForestQueryIter>
      get_forest_query_iter(const std::vector<float>& query_tensor, const LSH::Metric& metric) const;

    std::vector<std::pair<uint64_t, float>> query_top_k(const std::vector<float>& query_tensor, const LSH::Metric& metric, uint64_t k) const;

private:
    FileId tensors_file_id;

    const std::string mapping_path;
    const std::string index_path;

    std::unique_ptr<TensorBufferManager> tensor_buffer_manager;

    // Vector index
    std::unique_ptr<LSH::ForestIndex> forest_index;

    // Mapping between object id and its tensor bytes offset in the file (bytes)
    std::unordered_map<uint64_t, uint64_t> object_id2tensor_offset;

    // Serialize the tensor store (mapping and forest index if it exists)
    void serialize() const;

    // Deserialize the tensor store
    void deserialize();

    void set_tensor_buffer_manager();
};
