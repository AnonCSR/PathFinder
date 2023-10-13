/*
 * Tree is the data structure used for storing the data for LSH ForestIndex. It is a full binary tree that stores
 * buckets of object_ids in the leaves. In order to descend the tree with a given tensor, it must be hashed at each
 * level with a random hyperplane for choosing the direction. Each node will always have either zero or two children.
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

class TensorStore;

namespace LSH {

class Tree {
public:
    // Define the random engine types
    typedef boost::mt19937                                                   RandomGeneratorType;
    typedef boost::normal_distribution<float>                                DistributionType;
    typedef boost::variate_generator<RandomGeneratorType&, DistributionType> RandomEngineType;

    /*
     * Node represents a node in the tree. It stores a pointer to its parent and two children. If the node is a
     * leaf, it will have no children and a possibly non-empty bucket of object_ids.
     */
    struct Node {
        // Neighborhood references
        Node* parent;
        Node* children[2];
        // Bucket data
        std::vector<uint64_t> object_ids;

        Node(Node* parent) :
            parent (parent),
            children { nullptr, nullptr } { }

        ~Node() {
            delete children[0];
            delete children[1];
        }

        bool is_root() const {
            return parent == nullptr;
        }

        bool is_leaf() const {
            // Checking for a single child is enought since each node must have either 0 or 2 children
            return children[0] == nullptr;
        }

        Node* sibling() const {
            assert(!is_root());
            uint8_t sibling_index = parent->children[0] == this;
            return parent->children[sibling_index];
        }
    };

    // Return all the object_ids stored in the node descendants
    static std::vector<uint64_t> descendants(Node* node);

    // Initialize a new tree
    Tree(uint64_t max_depth, const TensorStore& tensor_store, RandomEngineType& random_engine);

    // Load an existing serialized tree from a filestream
    Tree(std::fstream& fs, const TensorStore& tensor_store);

    ~Tree();

    // Insert a new object_id into the tree
    void insert(uint64_t object_id);

    // Get the leaf with the longest prefix match and its depth for the hashed query tensor
    std::pair<Node*, uint64_t> descend(const std::vector<float>& query_tensor) const;

    // Hashes a tensor with a given depth (for choosing the random hyperplane). Returns 1 if the inner product is
    // non-negative, 0 otherwise
    uint8_t hash(const std::vector<float>& tensor, uint64_t depth) const;

    // Serialize the tree into a filestream
    void serialize(std::fstream& fs) const;

private:
    uint64_t           max_depth;
    uint64_t           tensors_dim;
    const TensorStore& tensor_store;

    Node* root;

    // Plane normals for each depth
    std::vector<std::vector<float>> plane_normals;

    // Internal recursive insertion algorithm
    void insert_recursive(uint64_t object_id, uint64_t depth, Node* node);

    // Deserialize the tree from a filestream
    void deserialize(std::fstream& fs);
};
} // namespace LSH
