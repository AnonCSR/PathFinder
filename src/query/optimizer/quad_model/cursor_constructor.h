#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "graph_models/object_id.h"
#include "query/executor/cursor_query_executor/mql/cursor_query_executor.h"
#include "query/query_context.h"
#include "query/parser/op/mql/ops.h"
#include "query/var_id.h"

namespace MQL {
class CursorConstructor : public OpVisitor {
public:
    std::unique_ptr<CursorQueryExecutor> executor;

    // Contains mandatory equalities of variables with constants
    // obtained from SET statement
    std::map<VarId, ObjectId> set_vars;

    // possible Logical Plan roots
    void visit(OpDescribe&) override;
    void visit(OpReturn&)   override;
    void visit(OpSet&)      override;
};
} // namespace MQL
