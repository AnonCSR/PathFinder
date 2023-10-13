#include "forest_query_iter.h"

#include "storage/index/tensor_store/lsh/forest_index.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace LSH;

ForestQueryIter::ForestQueryIter(
    const std::vector<float>& query_tensor_,
    const Metric&             metric_,
    const ForestIndex&        forest_index_
) :
    query_tensor (query_tensor_),
    metric       (metric_),
    forest_index (forest_index_)
{
    // Initialize containers
    uint64_t num_trees = forest_index.trees.size();
    nodes.reserve(num_trees);
    depths.reserve(num_trees);
    current_buckets.reserve(num_trees);
    for (const auto& tree : forest_index.trees) {
        const auto& [leaf, depth] = tree->descend(query_tensor);
        nodes.push_back(leaf);
        depths.push_back(depth);
        current_buckets.push_back(std::unordered_set<uint64_t>(leaf->object_ids.begin(), leaf->object_ids.end()));
    }
    current_maximum_depth = *std::max_element(depths.begin(), depths.end());
    intersect_buckets();
    if (!intersection_bucket.empty())
        fill_return_stack();
}


bool ForestQueryIter::next() {
    while (!return_stack.empty() || current_maximum_depth > 0) {
        // Return elements from the return_queue
        if (!return_stack.empty()) {
            current = return_stack.back();
            return_stack.pop_back();
            return true;
        }

        // Fill the stack
        while (current_maximum_depth > 0 && intersection_bucket.empty()) {
            // Extend buckets for current depth
            for (uint64_t i = 0; i < nodes.size(); i++) {
                if (depths[i] == current_maximum_depth) {
                    Tree::Node*           sibling             = nodes[i]->sibling();
                    std::vector<uint64_t> sibling_descendants = Tree::descendants(sibling);
                    current_buckets[i].insert(sibling_descendants.begin(), sibling_descendants.end());
                    nodes[i] = nodes[i]->parent;
                    depths[i]--;
                }
            }
            current_maximum_depth--;
            intersect_buckets();
        }
        if (!intersection_bucket.empty())
            fill_return_stack();
    }
    return false;
}


void ForestQueryIter::intersect_buckets() {
    // Start defining the intersection as the first bucket
    intersection_bucket = current_buckets[0];
    for (uint64_t i = 1; i < current_buckets.size(); i++) {
        // For each other bucket, remove the elements that are not present
        for (auto it = intersection_bucket.begin(); it != intersection_bucket.end(); ) {
            if (current_buckets[i].find(*it) == current_buckets[i].end())
                it = intersection_bucket.erase(it);
            else
                it++;
        }
    }
}


void ForestQueryIter::fill_return_stack() {
    assert(!intersection_bucket.empty());
    // Remove the intersection from all the current buckets to avoid duplicates
    for (auto& bucket : current_buckets) {
        for (const auto& object_id : intersection_bucket)
            bucket.erase(object_id);
    }
    // Fill the queue
    for (const auto& object_id : intersection_bucket) {
        float distance = metric.distance(query_tensor, forest_index.tensor_store.get(object_id));
        return_stack.emplace_back(object_id, distance);
    }
    intersection_bucket.clear();
    // Sort by the distance metric
    std::sort(
        return_stack.begin(),
        return_stack.end(),
        [&](const std::pair<uint64_t, float>& a, const std::pair<uint64_t, float>& b) {
            return metric.compare(a.second, b.second);
        });
}
