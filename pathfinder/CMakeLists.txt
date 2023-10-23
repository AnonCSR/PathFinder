cmake_minimum_required(VERSION 3.12)

# Set project name and version
project(PathFinder VERSION 0.2.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable export of compile commands output
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(EXECUTABLE_OUTPUT_PATH bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

# Define the compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -pthread -march=native -funroll-loops -fno-operator-names")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=undefined,address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g0")

# Enable interprocedural optimization if supported and BUILD_TYPE is RELEASE
string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
if(BUILD_TYPE STREQUAL "RELEASE")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_result OUTPUT ipo_output)
    if(ipo_result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO is not supported: ${ipo_output}")
    endif(ipo_result)
endif(BUILD_TYPE STREQUAL "RELEASE")

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Define sources
file(GLOB_RECURSE SRCS src/*.cc)
file(GLOB_RECURSE BIN_SRCS src/bin/*.cc)
file(GLOB_RECURSE TEST_SRCS src/tests/*.cc)
file(GLOB_RECURSE API_SRCS src/api/*.cc)
file(GLOB_RECURSE PY_API_SRCS src/api/python/*.cc)
# Remove unwanted files from common sources
list(REMOVE_ITEM SRCS ${BIN_SRCS})
list(REMOVE_ITEM SRCS ${TEST_SRCS})
list(REMOVE_ITEM SRCS ${PY_API_SRCS})

# Third party dependencies
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/antlr4-runtime-4.9.3)
include_directories(${CMAKE_SOURCE_DIR}/third_party/antlr4-runtime-4.9.3/runtime/src)
include_directories(${CMAKE_SOURCE_DIR}/third_party/boost_1_81/include)

# Define targets
add_library(pathfinder STATIC ${SRCS})
target_compile_definitions(pathfinder PUBLIC SERD_STATIC)
set(BUILD_TARGETS
    pf-import
    pf-server
    pf-query-mql
    pf-cli
)

# Build targets
foreach(target ${BUILD_TARGETS})
    add_executable(${target} ${CMAKE_SOURCE_DIR}/src/bin/${target}.cc)
    target_link_libraries(${target}
        pathfinder
        antlr4_cpp_runtime
        stdc++fs
        ssl
        crypto
        ncursesw
    )
endforeach(target)
