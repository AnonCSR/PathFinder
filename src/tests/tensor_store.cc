#include "storage/index/tensor_store/tensor_store.h"

#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include <filesystem>
#include <vector>

typedef int TestFunction();

static const std::string DB_DIR            = "./tests/tmp-db";
static const std::string TENSOR_STORE_NAME = "tmp-tensor-store";
static const uint64_t    TENSORS_DIM       = 10;
static const uint64_t    NUM_TENSORS       = 100'000;


int create_tensor_store() {
    auto tensor_store = TensorStore(TENSOR_STORE_NAME, TENSORS_DIM);
    if (!TensorStore::exists(TENSOR_STORE_NAME))
        return 1;
    return 0;
}

int load_empty_tensor_store() {
    auto tensor_store = TensorStore(TENSOR_STORE_NAME);
    if (tensor_store.tensors_dim != TENSORS_DIM)
        return 1;
    if (tensor_store.size() != 0)
        return 1;
    return 0;
}

int insert_tensor_store() {
    auto tensor_store = TensorStore(TENSOR_STORE_NAME);
    for (uint64_t i = 0; i < NUM_TENSORS; i++)
        tensor_store.insert(i, std::vector<float>(TENSORS_DIM, i));
    if (tensor_store.size() != NUM_TENSORS)
        return 1;
    // Test for boundaries
    if (tensor_store.get(0) != std::vector<float>(TENSORS_DIM, 0))
        return 1;
    if (tensor_store.get(NUM_TENSORS - 1) != std::vector<float>(TENSORS_DIM, NUM_TENSORS - 1))
        return 1;
    return 0;
}

int load_non_empty_tensor_store() {
    auto tensor_store = TensorStore(TENSOR_STORE_NAME);
    if (tensor_store.tensors_dim != TENSORS_DIM)
        return 1;
    if (tensor_store.size() != NUM_TENSORS)
        return 1;
    // Test for boundaries
    if (tensor_store.get(0) != std::vector<float>(TENSORS_DIM, 0))
        return 1;
    if (tensor_store.get(NUM_TENSORS - 1) != std::vector<float>(TENSORS_DIM, NUM_TENSORS - 1))
        return 1;
    return 0;
}

int main() {
    // Check if DB_DIR already exists
    if (Filesystem::exists(DB_DIR))
        std::filesystem::remove_all(DB_DIR);

    // Initialize FileManager
    FileManager::init(DB_DIR);

    // Insert tests
    std::vector<TestFunction*> tests;
    tests.push_back(&create_tensor_store);
    tests.push_back(&load_empty_tensor_store);
    tests.push_back(&insert_tensor_store);
    tests.push_back(&load_non_empty_tensor_store);

    // Run tests
    for (const auto& test_func : tests) {
        if (test_func())
            return 1;
    }


    // Destruction of FileManager
    file_manager.~FileManager();
    // Cleanup
    std::filesystem::remove_all(DB_DIR);

    return 0;
}