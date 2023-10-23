#pragma once

#include <boost/asio.hpp>

#include "api/graph_object.h"

class ResultParser {
enum class TYPE {
    NULL_T = 0x00,
    STR    = 0x01,
    INT64  = 0x02,
    FLOAT  = 0x03,
};

enum class STATE {
    EXPECTING_TYPE,
    READING_STR,
    READING_INT64,
    READING_FLOAT,
};

public:
    std::vector<std::vector<GraphObject>>& results;
    uint_fast32_t binding_size;

    STATE state = STATE::EXPECTING_TYPE;
    TYPE current_type = TYPE::NULL_T;

    std::string current_string;

    // used for int and float
    alignas(8) unsigned char small_buffer[8];

    uint_fast32_t small_buffer_pos;

    std::vector<GraphObject> current_result;

    ResultParser(std::vector<std::vector<GraphObject>>& results, uint_fast32_t binding_size) :
        results      (results),
        binding_size (binding_size) { }

    void check_end_binding() {
        if (current_result.size() == binding_size) {
            results.push_back(std::move(current_result));
        }
    }

    void add_int64() {
        int64_t i;
        uint8_t* dest = reinterpret_cast<uint8_t*>(&i);
        dest[0] = small_buffer[0];
        dest[1] = small_buffer[1];
        dest[2] = small_buffer[2];
        dest[3] = small_buffer[3];
        dest[4] = small_buffer[4];
        dest[5] = small_buffer[5];
        dest[6] = small_buffer[6];
        dest[7] = small_buffer[7];
        current_result.push_back(i);
        check_end_binding();
    }

    void add_float() {
        float f;
        uint8_t* dest = reinterpret_cast<uint8_t*>(&f);
        dest[0] = small_buffer[0];
        dest[1] = small_buffer[1];
        dest[2] = small_buffer[2];
        dest[3] = small_buffer[3];
        current_result.push_back(f);
        check_end_binding();
    }

    void process_byte(unsigned char byte) {
        switch (state) {
        case STATE::EXPECTING_TYPE: {
            current_type = static_cast<TYPE>(byte);
            switch (current_type) {
            case TYPE::NULL_T:
                current_result.push_back(std::monostate());
                check_end_binding();
                break;
            case TYPE::STR:
                state = STATE::READING_STR;
                break;
            case TYPE::INT64:
                state = STATE::READING_INT64;
                small_buffer_pos = 0;
                break;
            case TYPE::FLOAT:
                state = STATE::READING_FLOAT;
                small_buffer_pos = 0;
                break;
            }
            break;
        }
        case STATE::READING_STR: {
            if (byte != 0) {
                current_string.push_back(byte);
            } else {
                current_result.push_back(std::move(current_string));
                check_end_binding();
                state = STATE::EXPECTING_TYPE;
            }
            break;
        }
        case STATE::READING_INT64: {
            small_buffer[small_buffer_pos] = byte;
            small_buffer_pos++;
            if (small_buffer_pos == 8) {
                add_int64();
                state = STATE::EXPECTING_TYPE;
            }
            break;
        }
        case STATE::READING_FLOAT: {
            small_buffer[small_buffer_pos] = byte;
            small_buffer_pos++;
            if (small_buffer_pos == 4) {
                add_float();
                state = STATE::EXPECTING_TYPE;
            }
            break;
        }

        } // end switch
    }

    // TODO: is it really necessary?
    void end() {

    }
};
