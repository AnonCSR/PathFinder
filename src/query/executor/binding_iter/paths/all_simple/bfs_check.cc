#include "bfs_check.h"

#include <cassert>

#include "query/var_id.h"
#include "query/executor/binding_iter/paths/path_manager.h"

using namespace std;
using namespace Paths::AllSimple;

template <bool CYCLIC>
void BFSCheck<CYCLIC>::_begin(Binding& _parent_binding) {
    parent_binding = &_parent_binding;
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
}


template <bool CYCLIC>
bool BFSCheck<CYCLIC>::_next() {
    // Check if first state is final
    if (first_next) {
        first_next = false;
        auto& current_state = open.front();

        // Return false if node does not exist in the database
        if (!provider->node_exists(current_state.path_state->node_id.id)) {
            open.pop();
            return false;
        }

        // Starting state is solution
        if (current_state.path_state->node_id == end_object_id) {
            if (automaton.is_final_state[automaton.start_state]) {
                auto path_id = path_manager.set_path(current_state.path_state, path_var);
                parent_binding->add(path_var, path_id);
                if (!CYCLIC) {  // Acyclic can only have this trivial solution when start node = end node
                    queue<SearchState> empty;
                    open.swap(empty);
                }
                return true;
            } else if (!CYCLIC) {  // Acyclic can't have any more solutions when start node = end node
                queue<SearchState> empty;
                open.swap(empty);
                return false;
            }
        }
    }

    while (open.size() > 0) {
        auto& current_state = open.front();
        auto reached_final_state = expand_neighbors(current_state);

        // Enumerate reached solutions
        if (reached_final_state != nullptr) {
            auto path_id = path_manager.set_path(reached_final_state, path_var);
            parent_binding->add(path_var, path_id);
            return true;
        } else {
            // Pop and visit next state
            assert(iter->at_end());
            open.pop();
        }
    }
    return false;
}


template <bool CYCLIC>
const PathState* BFSCheck<CYCLIC>::expand_neighbors(const SearchState& current_state) {
    // Check if this is the first time that current_state is explored
    if (iter->at_end()) {
        current_transition = 0;
        // Check if automaton state has transitions
        if (automaton.from_to_connections[current_state.automaton_state].size() == 0) {
            return nullptr;
        }
        set_iter(current_state);
    }

    // Iterate over the remaining transitions of current_state
    // Don't start from the beginning, resume where it left thanks to current_transition and iter (pipeline)
    while (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
        auto& transition = automaton.from_to_connections[current_state.automaton_state][current_transition];

        // Iterate over records until a final state is reached
        while (iter->next()) {
            // Reconstruct path and check if it's simple, discard paths that are not simple
            if (!is_simple_path(current_state.path_state, ObjectId(iter->get_reached_node()))) {
                // If path can be cyclic, return solution only when the new node is the starting node and is also final
                if (CYCLIC && automaton.is_final_state[transition.to]) {
                    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
                    // This case only happens if the starting node and end node are the same
                    if (start_object_id == end_object_id && ObjectId(iter->get_reached_node()) == start_object_id) {
                        return visited.add(ObjectId(iter->get_reached_node()),
                                           transition.type_id,
                                           transition.inverse,
                                           current_state.path_state);
                    }
                }
                continue;
            }

            // Special Cases: End node has been reached
            if (ObjectId(iter->get_reached_node()) == end_object_id) {
                // Return only if it's a solution, never expand
                if (automaton.is_final_state[transition.to]) {
                    return visited.add(ObjectId(iter->get_reached_node()),
                                       transition.type_id,
                                       transition.inverse,
                                       current_state.path_state);
                }
                continue;
            }

            // Add new path state to visited
            auto new_visited_ptr = visited.add(ObjectId(iter->get_reached_node()),
                                               transition.type_id,
                                               transition.inverse,
                                               current_state.path_state);
            // Add new state to open
            open.emplace(new_visited_ptr, transition.to);
        }

        // Construct new iter with the next transition (if there exists one)
        current_transition++;
        if (current_transition < automaton.from_to_connections[current_state.automaton_state].size()) {
            set_iter(current_state);
        }
    }
    return nullptr;
}


template <bool CYCLIC>
void BFSCheck<CYCLIC>::_reset() {
    // Empty open and visited
    queue<SearchState> empty;
    open.swap(empty);
    visited.clear();
    first_next = true;
    iter = make_unique<NullIndexIterator>();

    // Add starting states to open and visited
    ObjectId start_object_id = start.is_var() ? (*parent_binding)[start.get_var()] : start.get_OID();
    auto start_node_visited = visited.add(start_object_id, ObjectId(), false, nullptr);
    open.emplace(start_node_visited, automaton.start_state);

    // Store ID for end object
    end_object_id = end.is_var() ? (*parent_binding)[end.get_var()] : end.get_OID();
}


template <bool CYCLIC>
void BFSCheck<CYCLIC>::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}


template class Paths::AllSimple::BFSCheck<true>;
template class Paths::AllSimple::BFSCheck<false>;