#pragma once

#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>

class Serialization {
public:
    static void write_bool(std::fstream& fs, bool value);
    static void write_uint64(std::fstream& fs, uint64_t value);
    static void write_uint8_vec(std::fstream& fs, const std::vector<uint8_t>& vec);
    static void write_float_vec(std::fstream& fs, const std::vector<float>& vec);
    static void write_uint64_vec(std::fstream& fs, const std::vector<uint64_t>& vec);
    static void write_uint642uint64_unordered_map(std::fstream& fs, const std::unordered_map<uint64_t, uint64_t>& map);

    static bool                                   read_bool(std::fstream& fs);
    static uint64_t                               read_uint64(std::fstream& fs);
    static std::vector<uint8_t>                   read_uint8_vec(std::fstream& fs, uint64_t size);
    static std::vector<float>                     read_float_vec(std::fstream& fs, uint64_t size);
    static std::vector<uint64_t>                  read_uint64_vec(std::fstream& fs, uint64_t size);
    static std::unordered_map<uint64_t, uint64_t> read_uint642uint64_unordered_map(std::fstream& fs, uint64_t size);
};
