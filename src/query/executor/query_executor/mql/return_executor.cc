#include "return_executor.h"

#include "graph_models/inliner.h"
#include "query/executor/binding_iter/paths/path_manager.h"
#include "query/executor/query_executor/csv_ostream_escape.h"
#include "query/executor/query_executor/tsv_ostream_escape.h"
#include "storage/string_manager.h"
#include "storage/tmp_manager.h"

using namespace MQL;

template<ReturnType ret>
void ReturnExecutor<ret>::print(std::ostream& os, ObjectId oid) {
    if (ret == ReturnType::CSV) {
        CSVOstreamEscape csv_ostream_escape(os);
        std::ostream escaped_os(&csv_ostream_escape);
        print(os, escaped_os, oid);

    } else {
        TSVOstreamEscape tsv_ostream_escape(os);
        std::ostream escaped_os(&tsv_ostream_escape);
        print(os, escaped_os, oid);
    }
}

template<ReturnType ret>
void ReturnExecutor<ret>::print_path_node(std::ostream& os, ObjectId node_id) {
    os << "(";
    ReturnExecutor::print(os, os, node_id);
    os << ")";
}

template<ReturnType ret>
void ReturnExecutor<ret>::print_path_edge(std::ostream& os, ObjectId edge_id, bool inverse) {
    if (inverse) {
        os << "<-[:";
        ReturnExecutor::print(os, os, edge_id);
        os << "]-";
    } else {
        os << "-[:";
        ReturnExecutor::print(os, os, edge_id);
        os << "]->";
    }
}


template<ReturnType ret>
void ReturnExecutor<ret>::print(std::ostream& os, std::ostream& escaped_os, ObjectId oid) {
    const auto mask        = oid.id & ObjectId::TYPE_MASK;
    const auto unmasked_id = oid.id & ObjectId::VALUE_MASK;
    switch (mask) {
    case ObjectId::MASK_NULL: {
        os << "null";
        break;
    }
    case ObjectId::MASK_ANON_INLINED: {
        os << "_a" << unmasked_id;
        break;
    }
    // TODO: implement ObjectId::MASK_ANON_TMP if supported in the future
    // case ObjectId::MASK_ANON_TMP:
    case ObjectId::MASK_NAMED_NODE_INLINED: {
        Inliner::print_string_inlined<7>(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_NAMED_NODE_EXTERN: {
        string_manager.print(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_NAMED_NODE_TMP: {
        tmp_manager.print_str(os, unmasked_id);
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_INLINED: {
        os << '"';
        Inliner::print_string_inlined<7>(escaped_os, unmasked_id);
        os << '"';
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_EXTERN: {
        os << '"';
        string_manager.print(escaped_os, unmasked_id);
        os << '"';
        break;
    }
    case ObjectId::MASK_STRING_SIMPLE_TMP: {
        os << '"';
        tmp_manager.print_str(escaped_os, unmasked_id);
        os << '"';
        break;
    }
    case ObjectId::MASK_NEGATIVE_INT: {
        int64_t i = (~oid.id) & 0x00FF'FFFF'FFFF'FFFFUL;
        os << (i*-1);
        break;
    }
    case ObjectId::MASK_POSITIVE_INT: {
        int64_t i = unmasked_id;
        os << i;
        break;
    }
    case ObjectId::MASK_FLOAT: {
        float f;
        uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
        dest[0] =  oid.id        & 0xFF;
        dest[1] = (oid.id >> 8)  & 0xFF;
        dest[2] = (oid.id >> 16) & 0xFF;
        dest[3] = (oid.id >> 24) & 0xFF;

        os << f;
        break;
    }
    case ObjectId::MASK_BOOL: {
        os << (unmasked_id == 0 ? "false" : "true");
        break;
    }
    case ObjectId::MASK_EDGE: {
        os << "_e" << unmasked_id;
        break;
    }
    case ObjectId::MASK_PATH: {
        os << '[';
        path_manager.print(os, unmasked_id, &print_path_node, &print_path_edge);
        os << ']';
        break;
    }
    default:
        throw std::logic_error("Unmanaged mask in CSVReturnExecutor print: "
            + std::to_string(mask));
    }
}

template<ReturnType ret>
uint64_t ReturnExecutor<ret>::execute(std::ostream& os) {
    // print header
    auto it = projection_vars.cbegin();
    if (it != projection_vars.cend()) {
        std::string var_name = get_query_ctx().get_var_name(*it);
        if (var_name[0] == '.') {
            var_name.erase(0, 1);
        }
        os << var_name;
        ++it;
    }
    while (it != projection_vars.cend()) {
        if (ret == ReturnType::CSV) {
            os << ',';
        } else {
            os << '\t';
        }
        std::string var_name = get_query_ctx().get_var_name(*it);
        if (var_name[0] == '.') {
            var_name.erase(0, 1);
        }
        os << var_name;
        ++it;
    }
    os << "\n";

    uint64_t result_count = 0;
    Binding binding(get_query_ctx().get_var_size());
    for (auto&& [var, value] : set_vars) {
        binding.add(var, value);
    }

    iter->begin(binding);

    std::unique_ptr<std::ostream> escaped_os;
    std::unique_ptr<std::stringbuf> stream_escaper;
    if (ret == ReturnType::CSV) {
        stream_escaper = std::make_unique<CSVOstreamEscape>(os);
    } else {
        stream_escaper = std::make_unique<TSVOstreamEscape>(os);
    }
    escaped_os = std::make_unique<std::ostream>(stream_escaper.get());

    while (iter->next() && result_count < limit) {
        result_count++;
        auto it = projection_vars.cbegin();

        if (it != projection_vars.cend()) {
            auto value = binding[*it];
            print(os, *escaped_os, value);
            ++it;
        }
        while (it != projection_vars.cend()) {
            auto value = binding[*it];
            if (ret == ReturnType::CSV) {
                os << ',';
            } else {
                os << '\t';
            }
            print(os, *escaped_os, value);
            ++it;
        }
        os << "\n";
    }

    return result_count;
}

template<ReturnType ret>
void ReturnExecutor<ret>::analyze(std::ostream& os, bool print_stats, int indent) const {
    BindingIterPrinter printer(os, print_stats, indent);
    iter->accept_visitor(printer);
}


template class MQL::ReturnExecutor<MQL::ReturnType::CSV>;
template class MQL::ReturnExecutor<MQL::ReturnType::TSV>;
