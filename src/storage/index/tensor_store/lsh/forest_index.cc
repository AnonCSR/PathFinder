#include "forest_index.h"

#include <fstream>
#include <random>

#include "storage/index/tensor_store/lsh/forest_query_iter.h"
#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/lsh/tree.h"
#include "storage/index/tensor_store/serialization.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace LSH;

ForestIndex::ForestIndex(uint64_t num_trees_, uint64_t max_depth_, const TensorStore& tensor_store_) :
    num_trees    (num_trees_),
    max_depth    (max_depth_),
    tensor_store (tensor_store_)
{
    assert(num_trees > 0);
    assert(max_depth > 0);

    // Define the random engine
    std::random_device        rd;
    Tree::RandomGeneratorType random_generator(rd());
    Tree::DistributionType    distribution(0.0f, 1.0f);
    Tree::RandomEngineType    random_engine(random_generator, distribution);

    // Initialize the trees
    for (uint64_t i = 0; i < num_trees; i++)
        trees.push_back(std::make_unique<Tree>(max_depth, tensor_store, random_engine));
}


ForestIndex::ForestIndex(const std::string& path, const TensorStore& tensor_store_) :
    tensor_store (tensor_store_)
{
    // Load trees from file
    deserialize(path);
    assert(num_trees > 0);
    assert(max_depth > 0);
}


void ForestIndex::insert(uint64_t object_id) {
    assert(tensor_store.contains(object_id));
    for (auto& tree : trees)
        tree->insert(object_id);
}


std::unique_ptr<ForestQueryIter> ForestIndex::get_forest_query_iter(
    const std::vector<float>& query_tensor,
    const Metric&             metric) const
{
    assert(query_tensor.size() == tensor_store.tensors_dim);
    return std::make_unique<ForestQueryIter>(query_tensor, metric, *this);
}


std::vector<std::pair<uint64_t, float>> ForestIndex::query_top_k(
    const std::vector<float>& query_tensor,
    const Metric&             metric,
    uint64_t                  k) const
{
    assert(k > 0);
    // Current seen object_ids
    std::unordered_set<uint64_t> union_bucket;

    // Initialize containers
    uint64_t num_trees = trees.size();
    std::vector<Tree::Node*> nodes;
    std::vector<uint64_t> depths;
    nodes.reserve(num_trees);
    depths.reserve(num_trees);
    for (const auto& tree : trees) {
        const auto& [leaf, depth] = tree->descend(query_tensor);
        nodes.push_back(leaf);
        depths.push_back(depth);
        union_bucket.insert(leaf->object_ids.begin(), leaf->object_ids.end());
    }
    uint64_t current_maximum_depth = *std::max_element(depths.begin(), depths.end());

    // Fill the union_bucket
    while (current_maximum_depth > 0 && union_bucket.size() < k) {
        for (uint64_t i = 0; i < nodes.size(); i++) {
            if (depths[i] == current_maximum_depth) {
                Tree::Node*           sibling             = nodes[i]->sibling();
                std::vector<uint64_t> sibling_descendants = Tree::descendants(sibling);
                union_bucket.insert(sibling_descendants.begin(), sibling_descendants.end());
                nodes[i] = nodes[i]->parent;
                depths[i]--;
            }
        }
        current_maximum_depth--;
    }

    k = std::min(k, union_bucket.size());

    // Fill, sort and slice the result
    std::vector<std::pair<uint64_t, float>> result;
    result.reserve(union_bucket.size());
    for (const auto& object_id : union_bucket) {
        float distance = metric.distance(query_tensor, tensor_store.get(object_id));
        result.emplace_back(object_id, distance);
    }    
    std::partial_sort(
        result.begin(),
        result.begin() + k,
        result.end(),
        [&metric](const std::pair<uint64_t, float>& a, const std::pair<uint64_t, float>& b) {
            return metric.compare(b.second, a.second);
        });
    result.resize(k);
    return result;
}


void ForestIndex::serialize(const std::string& path) const {
    std::fstream ofs(path, std::ios::out | std::ios::binary | std::ios::trunc);
    // Serialize each tree adjacently
    Serialization::write_uint64(ofs, max_depth);
    Serialization::write_uint64(ofs, num_trees);
    for (auto& tree : trees)
        tree->serialize(ofs);
    assert(ofs.good());
    ofs.close();
}


void ForestIndex::deserialize(const std::string& path) {
    std::fstream ifs(path, std::ios::in | std::ios::binary);
    // Deserialize each tree
    max_depth = Serialization::read_uint64(ifs);
    num_trees = Serialization::read_uint64(ifs);
    for (uint64_t i = 0; i < num_trees; i++)
        trees.push_back(std::make_unique<Tree>(ifs, tensor_store));
    assert(ifs.good());
    ifs.close();
}
