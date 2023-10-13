#pragma once

#include <cstdint>
#include <stack>
#include <unordered_set>
#include <vector>

#include "storage/index/tensor_store/lsh/metric.h"
#include "storage/index/tensor_store/lsh/tree.h"

class TensorStore;

namespace LSH {
class ForestIndex;

/*
 * ForestQueryIter is a class for iterating over all the indexed object_ids in the forest given an arbitrary query
 * tensor. It is based in the SynchAscend algorithm found in the paper LSH Forest: Self-Tuning Indexes for Similarity
 * Search. For using this class, you would do something like:
 *   while (query_iter->next()) {
 *       // Do stuff with query_iter->current
 *   }
 */
class ForestQueryIter {
public:
    std::pair<uint64_t, float> current;

    ForestQueryIter(const std::vector<float>& query_tensor, const Metric& metric, const ForestIndex& forest_index);

    // Returns true if current has the next element, false otherwise
    bool next();

private:
    const std::vector<float> query_tensor;
    const Metric&            metric;
    const ForestIndex&       forest_index;

    uint64_t current_maximum_depth;

    std::vector<Tree::Node*> nodes;
    std::vector<uint64_t>    depths;

    // For each tree, contains all the object_ids that have been retrieved from them and have not been intersected yet
    std::vector<std::unordered_set<uint64_t>> current_buckets;

    // Container for the intersection
    std::unordered_set<uint64_t> intersection_bucket;

    // Stack with sorted pairs of object_id and distance
    std::vector<std::pair<uint64_t, float>> return_stack;

    // Intersect the current_buckets into the intersection_bucket
    void intersect_buckets();

    // Fills the stack with sorted pairs of object_id and distance from the intersection_bucket
    void fill_return_stack();
};
} // namespace LSH