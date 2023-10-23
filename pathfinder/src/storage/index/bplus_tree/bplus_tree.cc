#include "bplus_tree.h"

#include <cassert>

#include "macros/likely.h"
#include "query/exceptions.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/index/record.h"

template <std::size_t N>
BPlusTree<N>::BPlusTree(const std::string& name) :
    dir_file_id  (file_manager.get_file_id(name + ".dir")),
    leaf_file_id (file_manager.get_file_id(name + ".leaf")),
    root         (BPlusTreeDir<N>(leaf_file_id, buffer_manager.get_page(dir_file_id, 0))) { }


template <std::size_t N>
std::unique_ptr<BPlusTreeDir<N>> BPlusTree<N>::get_root() const noexcept {
    return std::make_unique<BPlusTreeDir<N>>(
        leaf_file_id,
        buffer_manager.get_page(dir_file_id, 0)
    );
}


template <std::size_t N>
BptIter<N> BPlusTree<N>::get_range(bool* interruption_requested,
                                   const Record<N>& min,
                                   const Record<N>& max) const noexcept {
    auto leaf_and_pos = root.search_leaf(min);
    return BptIter<N>(interruption_requested, std::move(leaf_and_pos), max);
}


template <std::size_t N>
void BPlusTree<N>::insert(const Record<N>& record) {
    root.insert(record);
}


template <std::size_t N>
bool BPlusTree<N>::check() const {
    return root.check();
}


/******************************* BptIter ********************************/
template <std::size_t N>
const Record<N>* BptIter<N>::next() {
    while (true) {
        if (PF_unlikely(*interruption_requested)) {
            throw InterruptedException();
        }
        if (current_pos < current_leaf.get_value_count()) {
            current_leaf.get_record(current_pos, &current_record);
            // check if res is less than max
            for (unsigned int i = 0; i < N; ++i) {
                if (current_record[i] < max[i]) {
                    ++current_pos;
                    return &current_record;
                }
                else if (current_record[i] > max[i]) {
                    return nullptr;
                }
                // continue iterating if current_record[i] == max[i]
            }
            ++current_pos;
            return &current_record; // res == max
        }
        else if (current_leaf.has_next()) {
            current_leaf.update_to_next_leaf();
            current_pos = 0;
            // continue while
        }
        else {
            return nullptr;
        }
    }
}


template class BPlusTree<1>;
template class BPlusTree<2>;
template class BPlusTree<3>;
template class BPlusTree<4>;

template class BptIter<1>;
template class BptIter<2>;
template class BptIter<3>;
template class BptIter<4>;
