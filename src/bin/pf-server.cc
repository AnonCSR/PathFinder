#include <iostream>

#include "graph_models/quad_model/quad_model.h"
#include "graph_models/rdf_model/rdf_model.h"
#include "network/mql/server.h"
#include "network/sparql/server.h"
#include "storage/buffer_manager.h"
#include "storage/file_manager.h"
#include "storage/filesystem.h"
#include "third_party/cli11/CLI11.hpp"


static uint64_t read_uint64(std::fstream& fs) {
    uint64_t res = 0;
    uint8_t buf[8];

    fs.read((char*)buf, sizeof(buf));

    for (int i = 0; i < 8; i++) {
        res |= static_cast<uint64_t>(buf[i]) << (i * 8);
    }

    if (!fs.good()) {
        throw std::runtime_error("Error reading uint64");
    }

    return res;
}


struct PathModeValidator : public CLI::Validator {
    PathModeValidator() {
        name_ = "path_mode_validator";
        func_ = [](std::string& str) -> std::string {
            str = CLI::detail::to_lower(str);
            if (str == "dfs" || str == "bfs")
                return "";
            else
                return std::string("expecting bfs|dfs");
        };
    }
};


int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);

    uint_fast32_t seconds_timeout = 60;
    uint_fast32_t port            = 8080;
    uint_fast32_t max_threads     = std::thread::hardware_concurrency();

    uint64_t limit          = 0;
    uint64_t load_strings   = 2ULL * 1024 * 1024 * 1024;
    uint64_t shared_buffer  = BufferManager::DEFAULT_SHARED_BUFFER_POOL_SIZE * Page::PF_PAGE_SIZE;
    uint64_t private_buffer = BufferManager::DEFAULT_PRIVATE_BUFFER_POOL_SIZE * Page::PF_PAGE_SIZE;

    std::string db_directory;
    std::string config_path;
    std::string path_mode;

    CLI::App app{"PathFinder Server"};
    app.get_formatter()->column_width(35);
    app.option_defaults()->always_capture_default();

    app.add_option("database", db_directory)
        ->description("Database directory")
        ->type_name("<path>")
        ->check(CLI::ExistingDirectory.description(""))
        ->required();

    app.add_option("-p,--port", port)
        ->description("Server port")
        ->type_name("<port>")
        ->check(CLI::Range(1024, 65'535).description(""));

    app.add_option("-t,--timeout", seconds_timeout)
        ->description("Query timeout in seconds")
        ->type_name("<seconds>")
        ->check(CLI::Range(1, 100'000).description(""));

    app.add_option("-l,--limit", limit)
        ->description("Maximum number of results returned\nPass 0 to set no limit")
        ->type_name("<rows>")
        ->check(CLI::Range(static_cast<uint64_t>(0), static_cast<uint64_t>(UINT64_MAX)).description(""));

    app.add_option("--config", config_path)
        ->description("Config file")
        ->type_name("<path>")
        ->check(CLI::ExistingFile.description(""));

    app.add_option("--threads", max_threads)
        ->description("Number of worker threads")
        ->type_name("<number>")
        ->check(CLI::Range(1, 128).description(""));

    app.add_option("--load-strings", load_strings)
        ->description("Total amount of strings to pre-load\nAllows units such as MB and GB")
        ->option_text("<bytes> [2GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--shared-buffer", shared_buffer)
        ->description("Size of buffer shared between threads\nAllows units such as MB and GB")
        ->option_text("<bytes> [1GB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--private-buffer", private_buffer)
        ->description("Size of private per-thread buffers,\nAllows units such as MB and GB")
        ->option_text("<bytes> [64MB]")
        ->transform(CLI::AsSizeValue(false))
        ->check(CLI::Range(1024ULL * 1024, 1024ULL * 1024 * 1024 * 1024));

    app.add_option("--path-mode", path_mode)
        ->description("Path Mode")
        ->option_text("bfs|dfs")
        ->transform(PathModeValidator());

    CLI11_PARSE(app, argc, argv);


    shared_buffer /= Page::PF_PAGE_SIZE;
    private_buffer /= Page::PF_PAGE_SIZE;

    if (!config_path.empty()) {
        if (!Filesystem::exists(config_path)) {
            std::cerr << "Config file does not exist: " << config_path << "\n";
            return 1;
        } else if (!Filesystem::is_regular_file(config_path)) {
            std::cerr << "Config file is not a file: " << config_path << "\n";
            return 1;
        }
        if (logger.read_config(config_path)) {
            return 1;
        }
    }


    auto catalog_path = db_directory + "/catalog.dat";
    auto catalog_fs = std::fstream(catalog_path, std::ios::in | std::ios::binary);
    if (!catalog_fs.is_open()) {
        std::cerr << "Could not open catalog: " << catalog_path << "\n";
        return 1;
    }
    auto model_identifier = read_uint64(catalog_fs);


    switch (model_identifier) {
        case QuadCatalog::MODEL_ID: {
            std::cout << "Initializing Quad Model server...\n";

            auto model_destroyer = QuadModel::init(db_directory,
                                                   load_strings,
                                                   shared_buffer,
                                                   private_buffer,
                                                   max_threads);
            if (limit != 0) {
                quad_model.MAX_LIMIT = limit;
            }

            if (!path_mode.empty()) {
                if (path_mode == "bfs")
                    quad_model.path_mode = PathMode::BFS;
                else if (path_mode == "dfs")
                    quad_model.path_mode = PathMode::DFS;
            }

            quad_model.catalog().print();

            MQL::Server server;
            server.run(port, max_threads, std::chrono::seconds(seconds_timeout));

            return EXIT_SUCCESS;
        }
        case RdfCatalog::MODEL_ID: {
            std::cout << "Initializing RDF Model server..." << std::endl;

            auto model_destroyer = RdfModel::init(db_directory,
                                                  load_strings,
                                                  shared_buffer,
                                                  private_buffer,
                                                  max_threads);

            if (limit != 0) {
                rdf_model.MAX_LIMIT = limit;
            }

            if (!path_mode.empty()) {
                if (path_mode == "bfs")
                    rdf_model.path_mode = PathMode::BFS;
                else if (path_mode == "dfs")
                    rdf_model.path_mode = PathMode::DFS;
            }

            rdf_model.catalog().print();

            SPARQL::Server server;
            server.run(port, max_threads, std::chrono::seconds(seconds_timeout));

            return EXIT_SUCCESS;
        }
        default: {
            std::cerr << "Unknow model identifier: " << model_identifier << "\n";
            return 1;
        }
    }
}
