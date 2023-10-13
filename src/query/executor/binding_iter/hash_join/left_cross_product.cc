#include "left_cross_product.h"

#include <cassert>

void LeftCrossProduct::_begin(Binding& _parent_binding) {
    this->parent_binding = &_parent_binding;
    lhs->begin(_parent_binding);
    rhs->begin(_parent_binding);

    // If's clauses organized to avoid a reset of lhs

    // Check if rhs is empty
    if (rhs->next()) {
        // Rhs non empty, then lhs must be non empty
        if (!lhs->next()) {
            // Lhs empty and rhs_iter = nullptr return false in next
            rhs_iter = nullptr;
        } else {
            // Rhs non empty, reset for enumerate all tuples in next
            rhs_iter = rhs.get();
            rhs_iter->reset();
        }
    } else {
        rhs_iter = nullptr;
        // Avoid set null in each iteration in next()
        for (auto& var : rhs_vars) {
            parent_binding->add(var, ObjectId::get_null());
        }
    }

}


bool LeftCrossProduct::_next() {
    // If rhs is empty, only return lhs tuples
    if (rhs_iter == nullptr) {
        if (lhs->next()) {
            return true;
        } else {
            return false;
        }
    }
    // Rhs non empty, for each tuple of lhs, return all rhs
    while (true) {
        if (rhs_iter->next()) {
            return true;
        } else {
            if (lhs->next()) {
                rhs_iter->reset();
            } else {
                return false;
            }
        }
    }
}


void LeftCrossProduct::_reset() {
    rhs->reset();
    lhs->reset();

    if (rhs->next()) {
        rhs_iter = rhs.get();
        rhs_iter->reset();
        lhs->next();
    } else {
        rhs_iter = nullptr;
        for (auto& var : rhs_vars) {
            parent_binding->add(var, ObjectId::get_null());
        }
    }

}


void LeftCrossProduct::assign_nulls() {
    rhs->assign_nulls();
    lhs->assign_nulls();
}


void LeftCrossProduct::accept_visitor(BindingIterVisitor& visitor) {
    visitor.visit(*this);
}
