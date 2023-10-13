#pragma once

#include <memory>

#include "query/executor/binding_iter/paths/index_provider/path_index.h"

namespace Paths { namespace Any {

// Dummy structure for template usage
class DummySet {
public:
    static inline void clear() { }
    static inline int end() {return 0;}
    static inline int find(uint64_t) {return 0;}
    static inline void insert(uint64_t) { }
};

struct DFSSearchState {
    const ObjectId node_id;
    const uint32_t state;
    uint32_t current_transition = 0;
    std::unique_ptr<EdgeIter> iter = std::make_unique<NullIndexIterator>();  // Iterator
    DFSSearchState(ObjectId node_id, uint32_t state) : node_id(node_id), state(state) { }
};

struct SearchState {
    // The ID of the node the algorithm has reached
    const ObjectId node_id;

    // Pointer to the previous SearchState that leads to the current one
    // (used to reconstruct paths)
    const SearchState* previous;

    // The type of the traversed edge
    // (used to reconstruct paths)
    const ObjectId type_id;

    // State of the automaton defining the path query
    const uint32_t automaton_state;

    // Indicates in which direction the edge was traversed
    // (the language allows traversing in both directions)
    const bool inverse_direction;

    SearchState(uint32_t           automaton_state,
                ObjectId           node_id,
                const SearchState* previous,
                bool               inverse_direction,
                ObjectId           type_id) :
        node_id           (node_id),
        previous          (previous),
        type_id           (type_id),
        automaton_state   (automaton_state),
        inverse_direction (inverse_direction) {}

    // For ordered set
    bool operator<(const SearchState& other) const {
        if (automaton_state < other.automaton_state) {
            return true;
        } else if (other.automaton_state < automaton_state) {
            return false;
        } else {
            return node_id < other.node_id;
        }
    }

    // For unordered set
    bool operator==(const SearchState& other) const {
        return automaton_state == other.automaton_state && node_id == other.node_id;
    }

    SearchState clone() const {
        return SearchState(automaton_state, node_id, previous, inverse_direction, type_id);
    }

    void print(std::ostream& os,
               void (*print_node) (std::ostream&, ObjectId),
               void (*print_edge) (std::ostream&, ObjectId, bool inverse),
               bool begin_at_left) const;

};

}} // namespace Paths::Any

// For unordered set
template<>
struct std::hash<Paths::Any::SearchState> {
    std::size_t operator() (const Paths::Any::SearchState& lhs) const {
        return lhs.automaton_state ^ lhs.node_id.id;
    }
};
