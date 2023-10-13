#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "storage/index/tensor_store/lsh/forest_query_iter.h"
#include "storage/index/tensor_store/lsh/metric.h"

class TensorStore;

namespace LSH {
class Tree;
/*
 * ForestIndex is the main class for creating an LSH Forest Index for a given TensorStore. After building the index, one
 * can iterate over all the indexed object_ids through a QueryIterator instance
 */
class ForestIndex {
    friend class ForestQueryIter;

public:
    // Initialize a new forest index
    ForestIndex(uint64_t num_trees, uint64_t max_depth, const TensorStore& tensor_store);

    // Load an existing forest index from a file
    ForestIndex(const std::string& path, const TensorStore& tensor_store);

    // Insert an object_id into the forest
    void insert(uint64_t object_id);

    // Get an iterator over all the object_ids in the forest
    std::unique_ptr<ForestQueryIter> get_forest_query_iter(
        const std::vector<float>& query_tensor,
        const Metric& metric) const;

    // Get the top k most similar object_ids from the forest using the SynchAscend algorithm
    std::vector<std::pair<uint64_t, float>> query_top_k(
        const std::vector<float>& query_tensor,
        const Metric& metric,
        uint64_t k) const;

    // Serialize the tree to a file
    void serialize(const std::string& path) const;

private:
    uint64_t           num_trees;
    uint64_t           max_depth;
    const TensorStore& tensor_store;

    std::vector<std::unique_ptr<Tree>> trees;

    // Deserialize the tree from a file
    void deserialize(const std::string& path);
};
} // namespace LSH