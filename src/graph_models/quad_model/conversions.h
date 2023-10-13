#pragma once

#include <cstdint>
#include <string>

#include "graph_models/inliner.h"
#include "graph_models/object_id.h"
#include "graph_models/rdf_model/datatypes/decimal.h"
#include "graph_models/rdf_model/datatypes/decimal_inlined.h"
#include "graph_models/rdf_model/rdf_model.h"


namespace MQL {
class Conversions {
public:
    // static constexpr uint64_t DECIMAL_SIGN_MASK      = 0x0080'0000'0000'0000UL;
    // static constexpr uint64_t DECIMAL_NUMBER_MASK    = 0x007F'FFFF'FFFF'FFF0UL;
    // static constexpr uint64_t DECIMAL_SEPARATOR_MASK = 0x0000'0000'0000'000FUL;
    // static constexpr uint64_t FLOAT_SIGN_MASK        = 0x0000'0000'8000'0000UL;

    static constexpr int64_t  INTEGER_MAX            = 0x00FF'FFFF'FFFF'FFFFL;

    // The order, int < flt < inv is important
    static constexpr uint8_t OPTYPE_INTEGER = 0x01;
    static constexpr uint8_t OPTYPE_FLOAT   = 0x02;
    static constexpr uint8_t OPTYPE_INVALID = 0x03;

    /**
    *  @brief Unpacks an int (positive or negative) inside an ObjectId.
    *  @param oid The ObjectId to unpack.
    *  @return The value inside the ObjectId.
    */
    static int64_t unpack_int(ObjectId oid) {
        switch (oid.get_type()) {
        case ObjectId::MASK_NEGATIVE_INT:
            return static_cast<int64_t>(~oid.id & ObjectId::VALUE_MASK) * -1;
        case ObjectId::MASK_POSITIVE_INT:
            return static_cast<int64_t>(oid.get_value());;
        default:
            throw LogicException("Called unpack_int with incorrect ObjectId type, this should never happen");
        }
    }

    /**
    *  @brief Unpacks the float inside an ObjectId.
    *  @param oid The ObjectId to unpack.
    *  @return The value contained in the ObjectId.
    */
    static float unpack_float(ObjectId oid) {
        assert(oid.get_type() == ObjectId::MASK_FLOAT);

        auto  value = oid.id;
        float flt;
        auto  dst = reinterpret_cast<char*>(&flt);

        dst[0] = value & 0xFF;
        dst[1] = (value >> 8) & 0xFF;
        dst[2] = (value >> 16) & 0xFF;
        dst[3] = (value >> 24) & 0xFF;

        return flt;
    }
    // static Decimal unpack_decimal(ObjectId oid);
    // static float   unpack_float(ObjectId oid);
    // static double  unpack_double(ObjectId oid);

    // static std::string unpack_named_node(ObjectId oid);
    // static std::string unpack_string_simple(ObjectId oid);
    // static std::string unpack_string_xsd(ObjectId oid);
    // static std::pair<uint16_t, std::string> unpack_string_lang(ObjectId oid);
    // static std::pair<uint16_t, std::string> unpack_string_datatype(ObjectId oid);
    // static std::string unpack_iri(ObjectId oid);


    /**
    *  @brief Calculates the datatype that should be used for expression evaluation.
    *  @param oid1 ObjectId of the first operand.
    *  @param oid2 ObjectId of the second operand.
    *  @return datatype that should be used or OPTYPE_INVALID if not both operands are numeric.
    */
    static uint8_t calculate_optype(ObjectId oid1, ObjectId oid2) {
        return std::max(calculate_optype(oid1), calculate_optype(oid2));
    }

    /**
    *  @brief Calculates the generic datatypes of the operand in an expression.
    *  @param oid ObjectId of the operand involved in an expression.
    *  @return generic numeric datatype of the operand or OPTYPE_INVALID if oid is not numeric
    */
    static uint8_t calculate_optype(ObjectId oid) {
        switch (oid.get_sub_type()) {
        case ObjectId::MASK_INT:     return OPTYPE_INTEGER;
        case ObjectId::MASK_FLOAT:   return OPTYPE_FLOAT;
        default:                     return OPTYPE_INVALID;
        }
    }

    // static ObjectId pack_iri(const std::string& str);
    // static ObjectId pack_string_simple(const std::string& str);
    // static ObjectId pack_string_xsd(const std::string& str);
    // static ObjectId pack_string_lang(uint16_t lang, const std::string& str);
    // static ObjectId pack_string_datatype(uint16_t dt, const std::string& str);
    // static ObjectId pack_int(int64_t i);
    // static ObjectId pack_decimal(Decimal dec);
    // static ObjectId pack_float(float flt);
    // static ObjectId pack_double(double dbl);
    /**
    *  @brief Packs an int64_t into an ObjectId.
    *  @param dec The int64_t value that should be packed.
    *  @return An ObjectId (positive_integer or negative_integer) containing the value or null if it does not fit.
    */
    static ObjectId pack_int(int64_t i) {
        uint64_t mask = ObjectId::MASK_POSITIVE_INT;

        if (i < 0) {
            mask = ObjectId::MASK_NEGATIVE_INT;
            i *= -1;
            if (i > INTEGER_MAX) {
                return ObjectId::get_null();
            }
            i = (~i) & ObjectId::VALUE_MASK;
        } else {
            if (i > INTEGER_MAX) {
                return ObjectId::get_null();
            }
        }
        return ObjectId(mask | i);
    }


    /**
    *  @brief Packs a float into an ObjectId.
    *  @param flt The float value that should be packed.
    *  @return An ObjectId containing the value.
    */
    static ObjectId pack_float(float flt) {
        auto src = reinterpret_cast<unsigned char*>(&flt);

        auto oid = ObjectId::MASK_FLOAT;
        oid |= static_cast<uint64_t>(src[0]);
        oid |= static_cast<uint64_t>(src[1]) << 8;
        oid |= static_cast<uint64_t>(src[2]) << 16;
        oid |= static_cast<uint64_t>(src[3]) << 24;

        return ObjectId(oid);
    }

    // static int64_t     to_integer(ObjectId oid);
    // static Decimal     to_decimal(ObjectId oid);
    // static float       to_float(ObjectId oid);
    /**
    *  @brief Converts an ObjectId to float if permitted.
    *  @param oid ObjectId to convert.
    *  @return a float representing the ObjectId.
    *  @throws LogicException if the ObjectId has no permitted type.
    */
    static float to_float(ObjectId oid) {
        switch (oid.get_sub_type()) {
        case ObjectId::MASK_INT:
            return unpack_int(oid);
        case ObjectId::MASK_FLOAT:
            return unpack_float(oid);
        default:
            throw LogicException("Called to_float with incorrect ObjectId type, this should never happen");
        }
    }
    // static double      to_double(ObjectId oid);
    // static ObjectId    to_boolean(ObjectId oid);

    // // Returns a string with the lexical representation of the value
    // static std::string to_lexical_str(ObjectId oid);

    // static int64_t unpack_positive_int(ObjectId oid);
    // static int64_t unpack_negative_int(ObjectId oid);

    // static Decimal unpack_decimal_inlined(ObjectId oid);
    // static Decimal unpack_decimal_extern_tmp(ObjectId oid);

private:
    // static std::string unpack_named_node_inlined(ObjectId oid);
    // static std::string unpack_named_node_extern_tmp(ObjectId oid);

    // static std::string unpack_string_simple_inlined(ObjectId oid);
    // static std::string unpack_string_simple_extern_tmp(ObjectId oid);

    // static std::string unpack_string_xsd_inlined(ObjectId oid);
    // static std::string unpack_string_xsd_extern_tmp(ObjectId oid);

    // static std::pair<uint16_t, std::string> unpack_string_lang_inlined(ObjectId oid);
    // static std::pair<uint16_t, std::string> unpack_string_lang_extern_tmp(ObjectId oid);

    // static std::pair<uint16_t, std::string> unpack_string_datatype_inlined(ObjectId oid);
    // static std::pair<uint16_t, std::string> unpack_string_datatype_extern_tmp(ObjectId oid);

    // static std::string unpack_iri_inlined(ObjectId oid);
    // static std::string unpack_iri_extern_tmp(ObjectId oid);
};
} // namespace SPARQL
