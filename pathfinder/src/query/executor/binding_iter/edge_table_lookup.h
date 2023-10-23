#pragma once

#include <memory>
#include <vector>
#include <variant>

#include "query/executor/binding_iter.h"
#include "storage/index/random_access_table/random_access_table.h"

class EdgeTableLookup : public BindingIter {
public:
    EdgeTableLookup(
        RandomAccessTable<3>& table,
        Id                    edge,
        Id                    from,
        Id                    to,
        Id                    type
    ) :
        table         (table),
        edge          (edge),
        from          (from),
        to            (to),
        type          (type) { }

    void accept_visitor(BindingIterVisitor& visitor) override;
    void _begin(Binding& parent_binding) override;
    bool _next() override;
    void _reset() override;
    void assign_nulls() override;

    uint64_t lookups = 0;

private:
    RandomAccessTable<3>& table;
    Id edge;
    Id from;
    Id to;
    Id type;

    // because the interface will call next() until returns false, this variable prevent giving
    // the same result multiple times
    bool already_looked;

    Binding* parent_binding;
};
