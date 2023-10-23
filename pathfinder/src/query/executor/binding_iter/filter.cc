#include "filter.h"

#include "graph_models/rdf_model/conversions.h"

void Filter::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    child_iter->begin(_parent_binding);
}

// TODO: this filter is designed for SPARQL, for MQL we should create a new one?
// or maybe just inject the behaviour to evaluate
// `!Conversions::to_boolean(evaluation).is_true()`
bool Filter::_next() {
    while (child_iter->next()) {
        bool pass_filters = true;
        for (auto& filter : filters) {
            auto evaluation = filter->eval(*parent_binding);
            if (!SPARQL::Conversions::to_boolean(evaluation).is_true()) {
                pass_filters = false;
                break;
            }
        }
        if (pass_filters) {
            return true;
        } else {
            filtered_results++;
        }
    }
    return false;
}


void Filter::_reset() {
    child_iter->reset();
}


void Filter::assign_nulls() {
    child_iter->assign_nulls();
}


void Filter::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
