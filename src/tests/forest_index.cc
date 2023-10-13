#include <cstdint>
#include <filesystem>
#include <random>
#include <string>

#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include "storage/index/tensor_store/lsh/forest_index.h"
#include "storage/index/tensor_store/lsh/tree.h"
#include "storage/index/tensor_store/tensor_store.h"

typedef int TestFunction();

static const std::string        DB_DIR            = "./tests/tmp-db";
static const std::string        TENSOR_STORE_NAME = "tmp-tensor-store";
static const uint64_t           TENSORS_DIM       = 10;
static const uint64_t           NUM_TENSORS       = 100'000;
static const uint64_t           NUM_TREES         = 2;
static const uint64_t           MAX_DEPTH         = 5;
static const uint64_t           TOP_K             = 100;
static const std::vector<float> QUERY_TENSOR(TENSORS_DIM, 0.5f);
std::random_device              rd;
LSH::Tree::RandomGeneratorType  random_generator(rd());
LSH::Tree::DistributionType     distribution(0.0f, 1.0f);
LSH::Tree::RandomEngineType     random_engine(random_generator, distribution);

void randomize_tensor(std::vector<float>& tensor) {
    for (auto& value : tensor)
        value = random_engine();
}

int create_and_populate_tensor_store() {
    TensorStore        tensor_store(TENSOR_STORE_NAME, TENSORS_DIM);
    std::vector<float> tensor(TENSORS_DIM);
    for (uint64_t i = 0; i < NUM_TENSORS; i++) {
        randomize_tensor(tensor);
        tensor_store.insert(i, tensor);
    }
    if (tensor_store.size() != NUM_TENSORS)
        return 1;
    return 0;
}

int query_top_k(TensorStore& tensor_store) { 
    auto metric            = LSH::CosineSimilarity();
    auto top_k = tensor_store.query_top_k(QUERY_TENSOR, metric, TOP_K);
    if (top_k.size() != TOP_K)
        return 1;
    return 0;
}

int query_iter(TensorStore& tensor_store) {
    auto metric            = LSH::CosineSimilarity();
    auto forest_query_iter = tensor_store.get_forest_query_iter(QUERY_TENSOR, metric);
    // Loop over the entire index
    uint64_t count = 0;
    while (forest_query_iter->next())
        count++;
    if (count != NUM_TENSORS)
        return 1;
    return 0;
}

int build_index_and_query() {
    // Create forest index
    auto tensor_store = TensorStore(TENSOR_STORE_NAME);
    tensor_store.build_forest_index(NUM_TREES, MAX_DEPTH);
    // Execute queries
    if (query_top_k(tensor_store))
        return 1;
    if (query_iter(tensor_store))
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
    tests.push_back(&create_and_populate_tensor_store);
    tests.push_back(&build_index_and_query);

    // Run tests
    for (const auto& test_func : tests) {
        if (test_func())
            return 1;
    }

    // Cleanup
    std::filesystem::remove_all(DB_DIR);

    // Destruction of FileManager
    file_manager.~FileManager();

    return 0;
}