#pragma once

#include <vector>
#include <memory>

#include "query/executor/binding_iter.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

class Filter : public BindingIter {
public:
    Filter(
        std::unique_ptr<BindingIter>                child,
        std::vector<std::unique_ptr<BindingExpr>>&& filters,
        bool                                        is_having_filter
    ) :
        filters          (std::move(filters)),
        child_iter       (std::move(child)),
        is_having_filter (is_having_filter) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;


    Binding* parent_binding;

    std::vector<std::unique_ptr<BindingExpr>> filters;

    std::unique_ptr<BindingIter> child_iter;

    // To differentiate normal Filter and Having
    const bool is_having_filter;

    // statistics
    uint_fast32_t filtered_results = 0;
};
