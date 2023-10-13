#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/binding_iter_printer.h"
#include "query/query_context.h"

namespace MQL {

class CursorQueryExecutor {
public:
    CursorQueryExecutor(
        std::unique_ptr<BindingIter> iter,
        std::map<VarId, ObjectId>&& set_vars,
        std::vector<VarId>&& projection_vars,
        uint64_t limit
    );

    ~CursorQueryExecutor() {
        // We always have a CursorQueryExecutor as the root of our physical query plans.
        // If that changes we might need to call path_manager.clear() somewhere else
        // (it needs to be called always at the destruction of the query and only once)
        path_manager.clear();
    }

    // returns how many results were obtained
    bool next();

    void print_current(std::ostream&);

    uint64_t get_projection_vars_size() const {
        return projection_vars.size();
    }

private:
    std::unique_ptr<BindingIter> iter;

    std::map<VarId, ObjectId> set_vars;

    std::vector<VarId> projection_vars;

    uint64_t limit;

    uint64_t result_count = 0;

    Binding binding;

    static void print(std::ostream& os, ObjectId oid);
    // virtual void analyze(std::ostream&, bool print_stats, int indent = 0) const = 0;
};
}
