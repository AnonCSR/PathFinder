#pragma once

#include <cassert>
#include <cmath>
#include <numeric>
#include <vector>

namespace LSH {

/*
 * Metric is an abstract class that defines how to compute the distance between two tensors and how to compare their
 * similarity values.
 */
class Metric {
public:
    // Get the distance between two tensors
    virtual float distance(const std::vector<float>& tensor1, const std::vector<float>& tensor2) const = 0;

    // Return true if value2 has a greater similarity than value1, false otherwise (value1 <_{sim} value2)
    virtual bool compare(float value1, float value2) const = 0;
};

class CosineSimilarity : public Metric {
public:
    float distance(const std::vector<float>& tensor1, const std::vector<float>& tensor2) const override {
        float dot    = std::inner_product(tensor1.begin(), tensor1.end(), tensor2.begin(), 0.0f);
        float norm_a = std::sqrt(std::inner_product(tensor1.begin(), tensor1.end(), tensor1.begin(), 0.0f));
        float norm_b = std::sqrt(std::inner_product(tensor2.begin(), tensor2.end(), tensor2.begin(), 0.0f));
        assert(norm_a > 0.0f && norm_b > 0.0f);
        return dot / (norm_a * norm_b);
    }

    bool compare(float value1, float value2) const override {
        return value1 < value2;
    }
};
} // namespace LSH