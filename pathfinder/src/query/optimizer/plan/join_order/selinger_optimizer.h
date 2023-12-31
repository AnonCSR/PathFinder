#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"
#include "query/query_context.h"
#include "query/optimizer/plan/plan.h"

class SelingerOptimizer {
public:
    SelingerOptimizer(const std::vector<std::unique_ptr<Plan>>& base_plans);
    ~SelingerOptimizer();

    std::unique_ptr<Plan> get_plan();

private:
    const std::size_t plans_size;

    std::unique_ptr<Plan>** optimal_plans;

    // from n elements choose r, returns how many combinations can be formed
    // returns 0 if r > n although is a math error (necessary to get_index)
    // only works for small numbers
    static uint64_t nCr(uint_fast32_t n, uint_fast32_t r);

    static uint_fast32_t get_index(bool* arr, uint_fast32_t size);
};
