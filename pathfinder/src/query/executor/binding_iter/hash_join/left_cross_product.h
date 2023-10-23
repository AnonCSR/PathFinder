#pragma once

#include <memory>
#include <vector>

#include "query/executor/binding_iter.h"

class LeftCrossProduct : public BindingIter {
public:
    LeftCrossProduct(
        std::unique_ptr<BindingIter> lhs,
        std::unique_ptr<BindingIter> rhs,
        std::vector<VarId>&&           rhs_vars
    ) :
        lhs               (std::move(lhs)),
        rhs               (std::move(rhs)),
        rhs_vars          (std::move(rhs_vars)) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    std::unique_ptr<BindingIter> lhs;
    std::unique_ptr<BindingIter> rhs;

private:
    BindingIter* rhs_iter;

    std::vector<VarId> rhs_vars;

    Binding* parent_binding;
};
