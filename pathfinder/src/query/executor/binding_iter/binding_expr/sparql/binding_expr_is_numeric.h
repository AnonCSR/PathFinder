#pragma once

#include <memory>

#include "graph_models/object_id.h"
#include "query/executor/binding_iter/binding_expr/binding_expr.h"

namespace SPARQL {
class BindingExprIsNumeric : public BindingExpr {
public:
    std::unique_ptr<BindingExpr> expr;

    BindingExprIsNumeric(std::unique_ptr<BindingExpr> expr) :
        expr (std::move(expr)) { }

    ObjectId eval(const Binding& binding) override {
        auto expr_oid = expr->eval(binding);

        if (expr_oid.is_null())
            return ObjectId::get_null();
        else
            return ObjectId(ObjectId::MASK_BOOL | static_cast<uint64_t>(expr_oid.is_numeric()));
    }

    void accept_visitor(BindingExprVisitor& visitor) override {
        visitor.visit(*this);
    }
};
} // namespace SPARQL
