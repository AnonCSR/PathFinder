#include "tree.h"

#include <cassert>
#include <fstream>
#include <numeric>

#include "storage/index/tensor_store/serialization.h"
#include "storage/index/tensor_store/tensor_store.h"

using namespace LSH;

Tree::Tree(uint64_t max_depth_, const TensorStore& tensor_store_, RandomEngineType& random_engine) :
    max_depth    (max_depth_),
    tensors_dim  (tensor_store_.tensors_dim),
    tensor_store (tensor_store_)
{
    // Initialize plane normals
    plane_normals.resize(max_depth, std::vector<float>(tensors_dim));
    for (auto& plane_normal : plane_normals) {
        for (auto& value : plane_normal) {
            value = random_engine();
        }
    }
    // Initialize root
    root = new Node(nullptr);
}


Tree::Tree(std::fstream& fs, const TensorStore& tensor_store_) :
    tensor_store (tensor_store_)
{
    // Initialize root
    root = new Node(nullptr);
    deserialize(fs);
}


Tree::~Tree() {
    delete root;
}


std::vector<uint64_t> Tree::descendants(Node* node) {
    std::vector<uint64_t>    descendants;
    std::vector<Tree::Node*> stack = { node };
    while (!stack.empty()) {
        Tree::Node* current_node = stack.back();
        stack.pop_back();
        if (current_node->is_leaf()) {
            descendants.insert(descendants.end(), current_node->object_ids.begin(), current_node->object_ids.end());
        } else {
            stack.push_back(current_node->children[0]);
            stack.push_back(current_node->children[1]);
        }
    }
    return descendants;
}


void Tree::insert(uint64_t object_id) {
    insert_recursive(object_id, 0, root);
}


std::pair<Tree::Node*, uint64_t> Tree::descend(const std::vector<float>& query_tensor) const {
    assert(query_tensor.size() == tensors_dim);
    Node*    current_node  = root;
    uint64_t current_depth = 0;
    // Descend until reaching a leaf node
    while (!current_node->is_leaf()) {
        uint8_t hashed = hash(query_tensor, current_depth);
        current_node   = current_node->children[hashed];
        current_depth++;
    }
    return std::make_pair(current_node, current_depth);
}


uint8_t Tree::hash(const std::vector<float>& tensor, uint64_t depth) const {
    assert(tensor.size() == tensors_dim);
    assert(depth < max_depth);
    return std::inner_product(tensor.begin(), tensor.end(), plane_normals[depth].begin(), 0.0f) > 0.0f;
}


void Tree::serialize(std::fstream& fs) const {
    // Serialize plane normals
    Serialization::write_uint64(fs, max_depth);
    Serialization::write_uint64(fs, tensors_dim);
    for (const auto& plane_normal : plane_normals)
        Serialization::write_float_vec(fs, plane_normal);

    // Serialize tree structure with a left-priority DFS
    std::vector<uint8_t> dfs_bytes;
    std::vector<Node*>   stack = { root };
    std::vector<Node*>   leaves;
    while (!stack.empty()) {
        Node* current = stack.back();
        stack.pop_back();
        if (current->is_leaf()) {
            dfs_bytes.push_back(1);
            leaves.push_back(current);
        } else {
            dfs_bytes.push_back(0);
            stack.push_back(current->children[1]);
            stack.push_back(current->children[0]);
        }
    }
    Serialization::write_uint64(fs, dfs_bytes.size());
    Serialization::write_uint8_vec(fs, dfs_bytes);

    // Serialize leaves
    for (const auto& leaf : leaves) {
        Serialization::write_uint64(fs, leaf->object_ids.size());
        Serialization::write_uint64_vec(fs, leaf->object_ids);
    }
    assert(fs.good());
}


void Tree::deserialize(std::fstream& fs) {
    // Deserialize plane normals
    max_depth   = Serialization::read_uint64(fs);
    tensors_dim = Serialization::read_uint64(fs);
    plane_normals.resize(max_depth);
    for (auto& plane_normal : plane_normals)
        plane_normal = Serialization::read_float_vec(fs, tensors_dim);

    // Deserialize tree structure and leaves
    auto dfs_bytes_size = Serialization::read_uint64(fs);
    auto dfs_bytes      = Serialization::read_uint8_vec(fs, dfs_bytes_size);

    std::vector<Node*> stack = { root };
    for (const auto& byte : dfs_bytes) {
        Node* current = stack.back();
        stack.pop_back();
        if (byte == 0) {
            // Internal node
            current->children[0] = new Node(current);
            current->children[1] = new Node(current);
            stack.push_back(current->children[1]);
            stack.push_back(current->children[0]);
        } else {
            // Leaf node
            auto object_ids_size = Serialization::read_uint64(fs);
            current->object_ids  = Serialization::read_uint64_vec(fs, object_ids_size);
        }
    }
}


void Tree::insert_recursive(uint64_t object_id, uint64_t depth, Node* node) {
    // Get the tensor from the tensor store
    std::vector<float> tensor = tensor_store.get(object_id);
    assert(tensor.size() == tensors_dim);
    if (node->is_leaf()) {
        // Reached a leaf node
        node->object_ids.push_back(object_id);
        if (depth < max_depth && node->object_ids.size() > 1) {
            // Split the node. Initialize children
            node->children[0] = new Node(node);
            node->children[1] = new Node(node);
            for (const auto& object_id : node->object_ids) {
                uint8_t hashed = hash(tensor, depth);
                Node*   child  = node->children[hashed];
                insert_recursive(object_id, depth + 1, child);
            }
            node->object_ids.clear();
        }
    } else {
        // Continue recursively
        uint8_t hashed = hash(tensor, depth);
        Node*   child  = node->children[hashed];
        insert_recursive(object_id, depth + 1, child);
    }
}
