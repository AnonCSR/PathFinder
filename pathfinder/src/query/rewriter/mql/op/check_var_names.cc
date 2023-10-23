#include "check_var_names.h"

#include "query/exceptions.h"
#include "query/parser/expr/mql_exprs.h"
#include "query/parser/op/mql/ops.h"

using namespace MQL;

void CheckVarNames::visit(OpGroupBy& op_group_by) {
    op_group_by.op->accept_visitor(*this);

    for (auto var : op_group_by.vars) {
        if (declared_vars.find(var) == declared_vars.end()) {
            throw QuerySemanticException("Variable \""
                + get_query_ctx().get_var_name(var)
                + "\" not declared");
        }
    }
}


void CheckVarNames::visit(OpReturn& op_return) {
    op_return.op->accept_visitor(*this);

    for (auto&& [var, expr] : op_return.projection) {
        if (expr) {
            CheckVarNamesExpr expr_visitor(declared_vars, declared_path_vars);
            expr->accept_visitor(expr_visitor);
        } else {
            if (declared_vars.find(var) == declared_vars.end()) {
                throw QuerySemanticException("Variable \""
                    + get_query_ctx().get_var_name(var)
                    + "\" not declared");
            }
        }
    }
}


void CheckVarNames::visit(OpOrderBy& op_order_by) {
    op_order_by.op->accept_visitor(*this);

    for (auto&& [var, expr] : op_order_by.items) {
        if (expr) {
            CheckVarNamesExpr expr_visitor(declared_vars, declared_path_vars);
            expr->accept_visitor(expr_visitor);
        } else {
            if (declared_vars.find(var) == declared_vars.end()) {
                throw QuerySemanticException("Variable \""
                    + get_query_ctx().get_var_name(var)
                    + "\" not declared");
            }
        }
    }
}


void CheckVarNames::visit(OpBasicGraphPattern& op_basic_graph_pattern) {
    auto insert_vars = [&](const std::set<VarId>& vars) {
        for (auto& var : vars) {
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \""
                                            + get_query_ctx().get_var_name(var)
                                            + "\". Paths must have an unique variable");
            }
        }
    };

    for (auto& label : op_basic_graph_pattern.labels) {
        insert_vars(label.get_all_vars());
    }
    for (auto& property : op_basic_graph_pattern.properties) {
        insert_vars(property.get_all_vars());
    }
    for (auto& edge : op_basic_graph_pattern.edges) {
        insert_vars(edge.get_all_vars());
    }
    for (auto& disjoint_var : op_basic_graph_pattern.disjoint_vars) {
        insert_vars(disjoint_var.get_all_vars());
    }

    for (auto& path : op_basic_graph_pattern.paths) {
        if (path.from.is_var()) {
            auto var = path.from.get_var();
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(var)
                                            + "\". Paths must have an unique variable");
            }
        }

        if (path.to.is_var()) {
            auto var = path.to.get_var();
            declared_vars.insert(var);
            if (declared_path_vars.find(var) != declared_path_vars.end()) {
                throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(var)
                                            + "\". Paths must have an unique variable");
            }
        }

        if (!declared_path_vars.insert(path.var).second) {
            throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(path.var)
                                         + "\". Paths must have an unique variable");
        }
        if (!declared_vars.insert(path.var).second) {
            throw QuerySemanticException("Duplicated path variable \"" + get_query_ctx().get_var_name(path.var)
                                         + "\". Paths must have an unique variable");
        }
    }
}


void CheckVarNames::visit(OpOptional& op_optional) {
    op_optional.op->accept_visitor(*this);
    for (auto& optional_child : op_optional.optionals) {
        optional_child->accept_visitor(*this);
    }
}


void CheckVarNames::visit(OpWhere& op_where) {
    op_where.op->accept_visitor(*this);
    CheckVarNamesExpr expr_visitor(declared_vars, declared_path_vars);
    op_where.expr->accept_visitor(expr_visitor);
}


void CheckVarNames::visit(OpSet& op_set) {
    op_set.op->accept_visitor(*this);
}


void CheckVarNames::visit(OpMatch& op_match) {
    op_match.op->accept_visitor(*this);

    for (auto op_property : op_match.optional_properties) {
        if (op_property.node.is_var()) {
            if (declared_vars.find(op_property.node.get_var()) == declared_vars.end()) {
                throw QuerySemanticException("Variable \""
                    + get_query_ctx().get_var_name(op_property.node.get_var())
                    + "\" not declared");
            }
        }
        if (op_property.value.is_var()) {
            declared_vars.insert(op_property.value.get_var());
        }
    }
}


/*************************** ExprVisitor ***************************/
void CheckVarNamesExpr::visit(ExprVar& expr) {
    if (declared_vars.find(expr.var) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \""
            + get_query_ctx().get_var_name(expr.var)
            + "\" used in WHERE is not declared in MATCH"
        );
    }
}


void CheckVarNamesExpr::visit(ExprVarProperty& expr) {
    if (declared_vars.find(expr.var_without_property) == declared_vars.end()) {
        throw QuerySemanticException(
            "Variable \""
            + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" used in WHERE is not declared in MATCH"
        );
    }
    if (declared_path_vars.find(expr.var_without_property) != declared_path_vars.end()) {
        throw QuerySemanticException(
            "Variable \""
            + get_query_ctx().get_var_name(expr.var_without_property)
            + "\" is a path and cannot have properties");
    }
}


void CheckVarNamesExpr::visit(ExprAddition& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprDivision& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprModulo& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprMultiplication& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprSubtraction& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprGreaterOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprGreater& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprLessOrEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprLess& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprNotEquals& expr) {
    expr.lhs->accept_visitor(*this);
    expr.rhs->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprUnaryMinus& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprUnaryPlus& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprNot& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprIs& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAnd& expr) {
    for (auto& e : expr.and_list) {
        e->accept_visitor(*this);
    }
}


void CheckVarNamesExpr::visit(ExprOr& expr) {
    for (auto& e : expr.or_list) {
        e->accept_visitor(*this);
    }
}


void CheckVarNamesExpr::visit(ExprAggAvg& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAggCountAll&) {
    // Do nothing
}


void CheckVarNamesExpr::visit(ExprAggCount&) {
    // Do nothing
}


void CheckVarNamesExpr::visit(ExprAggMax& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAggMin& expr) {
    expr.expr->accept_visitor(*this);
}


void CheckVarNamesExpr::visit(ExprAggSum& expr) {
    expr.expr->accept_visitor(*this);
}
