#include "cursor_query_executor.h"

#include "graph_models/inliner.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/query_executor/csv_ostream_escape.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

using namespace MQL;

constexpr uint8_t TYPE_NULL  = 0x00;
constexpr uint8_t TYPE_STR   = 0x01;
constexpr uint8_t TYPE_INT64 = 0x02;
constexpr uint8_t TYPE_FLOAT = 0x03;

CursorQueryExecutor::CursorQueryExecutor(
    std::unique_ptr<BindingIter> _iter,
    std::map<VarId, ObjectId>&&  _set_vars,
    std::vector<VarId>&&         _projection_vars,
    uint64_t                     limit
) :
    iter            (std::move(_iter)),
    set_vars        (std::move(_set_vars)),
    projection_vars (std::move(_projection_vars)),
    limit           (limit),
    binding         (get_query_ctx().get_var_size())
{
    for (auto&& [var, value] : set_vars) {
        binding.add(var, value);
    }
    iter->begin(binding);
}

void CursorQueryExecutor::print(std::ostream& os, ObjectId oid) {
    const auto mask        = oid.id & ObjectId::TYPE_MASK;
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
    switch (mask) {
    case ObjectId::MASK_NULL: {
        os.put(TYPE_NULL);
        break;
    }
    case ObjectId::MASK_ANON_INLINED: {
        os.put(TYPE_STR);
        os << "_a" << unmasked_id;
        os << '\0';
        break;
    }
    // TODO: implement ObjectId::MASK_ANON_TMP if supported in the future
    // case ObjectId::MASK_ANON_TMP:
    case ObjectId::MASK_NAMED_NODE_INLINED: {
        os.put(TYPE_STR);
        Inliner::print_string_inlined<7>(os, unmasked_id);
        os << '\0';
        break;
    }
    case ObjectId::MASK_NAMED_NODE_EXTERN: {
        os.put(TYPE_STR);
        string_manager.print(os, unmasked_id);
        os << '\0';
        break;
    }
    case ObjectId::MASK_NAMED_NODE_TMP: {
        os.put(TYPE_STR);
        tmp_manager.print_str(os, unmasked_id);
        os << '\0';
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        os.put(TYPE_STR);
        os << '"';
        Inliner::print_string_inlined<7>(os, unmasked_id);
        os << '"';
        os << '\0';
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        os.put(TYPE_STR);
        os << '"';
        string_manager.print(os, unmasked_id);
        os << '"';
        os << '\0';
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        os.put(TYPE_STR);
        os << '"';
        tmp_manager.print_str(os, unmasked_id);
        os << '"';
        os << '\0';
        break;
    }
    case ObjectId::MASK_NEGATIVE_INT: {
        os.put(TYPE_INT64);
        uint64_t neg_id = (~oid.id) & 0x00FF'FFFF'FFFF'FFFFUL;
        os.put( neg_id        & 0xFF);
        os.put((neg_id >> 8)  & 0xFF);
        os.put((neg_id >> 16) & 0xFF);
        os.put((neg_id >> 24) & 0xFF);
        os.put((neg_id >> 32) & 0xFF);
        os.put((neg_id >> 40) & 0xFF);
        os.put((neg_id >> 48) & 0xFF);
        os.put((neg_id >> 56) & 0xFF);

        break;
    }
    case ObjectId::MASK_POSITIVE_INT: {
        os.put(TYPE_INT64);
        os.put( oid.id        & 0xFF);
        os.put((oid.id >> 8)  & 0xFF);
        os.put((oid.id >> 16) & 0xFF);
        os.put((oid.id >> 24) & 0xFF);
        os.put((oid.id >> 32) & 0xFF);
        os.put((oid.id >> 40) & 0xFF);
        os.put((oid.id >> 48) & 0xFF);
        os.put((oid.id >> 56) & 0xFF);

        break;
    }
    case ObjectId::MASK_FLOAT: {
        os.put(TYPE_FLOAT);
        os.put( oid.id        & 0xFF);
        os.put((oid.id >> 8)  & 0xFF);
        os.put((oid.id >> 16) & 0xFF);
        os.put((oid.id >> 24) & 0xFF);
        break;
    }
    // TODO:
    // case ObjectId::MASK_BOOL: {
    //     os << (unmasked_id == 0 ? "false" : "true");
    //     break;
    // }
    case ObjectId::MASK_EDGE: {
        os.put(TYPE_STR);
        os << "_e" << unmasked_id;
        os << '\0';
        break;
    }
    // case ObjectId::MASK_PATH: {
    //     path_manager.print(os, unmasked_id, &print_path_node, &print_path_edge);
    //     break;
    // }
    default:
        throw std::logic_error("Unmanaged mask in CursorQueryExecutor print: "
            + std::to_string(mask));
    }
}

bool CursorQueryExecutor::next() {
    if (result_count == limit) return false;
    if (iter->next()) {
        result_count++;
        return true;
    }

    return false;
}


void CursorQueryExecutor::print_current(std::ostream& os) {
    for (auto& var : projection_vars) {
        print(os, binding[var]);
    }
}
