#pragma once

#include <ostream>

#include "query/executor/binding.h"
#include "query/executor/binding_iter/binding_expr/binding_expr_printer.h"
#include "query/executor/binding_iter_visitor.h"
#include "query/query_context.h"


// Abstract class
class BindingIter {
protected:
    virtual void _begin(Binding& parent_binding) = 0;
    virtual bool _next() = 0;
    virtual void _reset() = 0;

public:
    // Counts begins and resets
    uint64_t executions = 0;
    // Starts with -1 to account for the last next being false
    uint64_t results = -1;
    // uint64_t elapsed_time;

    virtual ~BindingIter() = default;

    // parent_binding is the input and the iter will write its results there.
    // It will look at the parent_binding to know the value of the assigned variables
    inline void begin(Binding& parent_binding) {
        _begin(parent_binding);
        executions++;
    }

    // Iterator starts again.
    // It will look at the parent_binding to know the value of the assigned variables
    inline void reset() {
        _reset();
        executions++;
    }

    // Returns true if there is a next binding or false otherwise.
    // It modifies the parent_binding to include the new results.
    inline bool next() {
        results++;
        return _next();
    }

    // Every var that the iter sets in the binding when next() returns true is set to null
    virtual void assign_nulls() = 0;

    virtual void accept_visitor(BindingIterVisitor&) = 0;
};
