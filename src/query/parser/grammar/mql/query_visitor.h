#pragma once

#include <memory>
#include <vector>

#include "antlr4-runtime.h"
#include "graph_models/object_id.h"
#include "query/query_context.h"
#include "query/var_id.h"
#include "query/parser/grammar/mql/autogenerated/MQL_ParserBaseVisitor.h"
#include "query/parser/op/mql/ops.h"
#include "query/parser/paths/regular_path_expr.h"

class Expr;

namespace MQL {

class OpBasicGraphPattern;

class QueryVisitor : public MQL_ParserBaseVisitor {
private:
    struct ReturnInfo {
        std::vector<std::pair<VarId, std::unique_ptr<Expr>>> items;
        bool distinct;
        uint64_t limit;
    };

    struct OrderByInfo {
        std::vector<std::pair<VarId, std::unique_ptr<Expr>>> items;

        // must have the same size as items
        std::vector<bool> ascending_order;
    };

    ReturnInfo return_info;

    OrderByInfo order_by_info;

    std::vector<std::pair<VarId, ObjectId>> set_items;

    // Properties used declared inside MATCH
    std::set<OpProperty> match_var_properties;

    std::vector<VarId> group_by_vars;

    std::unique_ptr<Expr> current_expr;

    std::unique_ptr<OpBasicGraphPattern> current_basic_graph_pattern;

    std::set<VarId> possible_disjoint_vars;

    std::unique_ptr<RegularPathExpr> current_path;

    std::vector<OpLabel> insert_labels;

    std::vector<OpProperty> insert_properties;

    std::vector<OpEdge> insert_edges;

    // to detect possible disjoint vars / terms
    // initialized false to avoid calling
    // current_basic_graph_pattern->add_disjoint_term (segfault)
    // when seeing a DESCRIBE query
    bool first_element_disjoint = false;

    bool current_path_inverse;

    Id last_node = ObjectId::get_null();

    Id saved_node = ObjectId::get_null();

    Id saved_edge = ObjectId::get_null();

    Id saved_type = ObjectId::get_null();

public:
    std::unique_ptr<Op> current_op;

    // Properties used outside MATCH (WHERE/GROUP BY/ORDER BY/RETURN)
    std::set<OpProperty> used_var_properties;

    virtual antlrcpp::Any visitDescribeQuery(MQL_Parser::DescribeQueryContext*) override;
    virtual antlrcpp::Any visitInsertQuery(MQL_Parser::InsertQueryContext* ctx) override;
    virtual antlrcpp::Any visitMatchQuery(MQL_Parser::MatchQueryContext* ctx) override;
    virtual antlrcpp::Any visitMatchStatement(MQL_Parser::MatchStatementContext* ctx) override;
    virtual antlrcpp::Any visitSetItem(MQL_Parser::SetItemContext* ctx) override;

    virtual antlrcpp::Any visitInsertLabelElement(MQL_Parser::InsertLabelElementContext* ctx) override;
    virtual antlrcpp::Any visitInsertPropertyElement(MQL_Parser::InsertPropertyElementContext* ctx) override;
    virtual antlrcpp::Any visitInsertEdgeElement(MQL_Parser::InsertEdgeElementContext* ctx) override;

    virtual antlrcpp::Any visitReturnList(MQL_Parser::ReturnListContext* ctx) override;
    virtual antlrcpp::Any visitReturnItemVar(MQL_Parser::ReturnItemVarContext* ctx) override;
    virtual antlrcpp::Any visitReturnItemAgg(MQL_Parser::ReturnItemAggContext* ctx) override;
    virtual antlrcpp::Any visitReturnItemCount(MQL_Parser::ReturnItemCountContext* ctx) override;
    virtual antlrcpp::Any visitReturnAll(MQL_Parser::ReturnAllContext* ctx) override;

    virtual antlrcpp::Any visitOrderByStatement(MQL_Parser::OrderByStatementContext* ctx) override;
    virtual antlrcpp::Any visitOrderByItemVar(MQL_Parser::OrderByItemVarContext*ctx) override;
    virtual antlrcpp::Any visitOrderByItemAgg(MQL_Parser::OrderByItemAggContext* ctx) override;
    virtual antlrcpp::Any visitOrderByItemCount(MQL_Parser::OrderByItemCountContext* ctx) override;

    virtual antlrcpp::Any visitGroupByStatement(MQL_Parser::GroupByStatementContext* ctx) override;
    virtual antlrcpp::Any visitGroupByItem(MQL_Parser::GroupByItemContext* ctx) override;

    virtual antlrcpp::Any visitGraphPattern(MQL_Parser::GraphPatternContext* ctx) override;
    virtual antlrcpp::Any visitBasicPattern(MQL_Parser::BasicPatternContext* ctx) override ;
    virtual antlrcpp::Any visitLinearPattern(MQL_Parser::LinearPatternContext* ctx) override;
    virtual antlrcpp::Any visitFixedNodeInside(MQL_Parser::FixedNodeInsideContext* ctx) override;
    virtual antlrcpp::Any visitVarNode(MQL_Parser::VarNodeContext* ctx) override;
    virtual antlrcpp::Any visitEdge(MQL_Parser::EdgeContext* ctx) override;
    virtual antlrcpp::Any visitEdgeInside(MQL_Parser::EdgeInsideContext* ctx) override;

    virtual antlrcpp::Any visitPath(MQL_Parser::PathContext* ctx) override;
    virtual antlrcpp::Any visitPathAlternatives(MQL_Parser::PathAlternativesContext* ctx) override;
    virtual antlrcpp::Any visitPathSequence(MQL_Parser::PathSequenceContext* ctx) override;
    virtual antlrcpp::Any visitPathAtomSimple(MQL_Parser::PathAtomSimpleContext* ctx) override;
    virtual antlrcpp::Any visitPathAtomAlternatives(MQL_Parser::PathAtomAlternativesContext* ctx) override;

    virtual antlrcpp::Any visitWhereStatement(MQL_Parser::WhereStatementContext* ctx) override;

    virtual antlrcpp::Any visitExprVar(MQL_Parser::ExprVarContext* ctx) override;
    virtual antlrcpp::Any visitExprValueExpr(MQL_Parser::ExprValueExprContext* ctx) override;

    virtual antlrcpp::Any visitConditionalOrExpr(MQL_Parser::ConditionalOrExprContext* ctx) override;
    virtual antlrcpp::Any visitConditionalAndExpr(MQL_Parser::ConditionalAndExprContext* ctx) override;
    virtual antlrcpp::Any visitComparisonExprOp(MQL_Parser::ComparisonExprOpContext* ctx) override;
    virtual antlrcpp::Any visitComparisonExprIs(MQL_Parser::ComparisonExprIsContext* ctx) override;
    virtual antlrcpp::Any visitAdditiveExpr(MQL_Parser::AdditiveExprContext* ctx) override;
    virtual antlrcpp::Any visitMultiplicativeExpr(MQL_Parser::MultiplicativeExprContext* ctx) override;
    virtual antlrcpp::Any visitUnaryExpr(MQL_Parser::UnaryExprContext* ctx) override;
};
} // namespace MQL
