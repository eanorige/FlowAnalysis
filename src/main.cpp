#include <filesystem>
#include <iostream>

#include "NoC.hpp"

int main(int argc, char* argv[]) {
    std::string config_path = "data/noc_config.json";
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [config_file.json]" << std::endl;
        return 1;
    } else if (argc > 1) {
        config_path = argv[1];
    }

    // Handle relative paths correctly if run from build dir
    if (!std::filesystem::exists(config_path)) {
        // Try looking one level up (common if running from build/)
        if (std::filesystem::exists("../" + config_path)) {
            config_path = "../" + config_path;
        } else {
            std::cerr << "Error: Configuration file not found at " << config_path << std::endl;
            return 1;
        }
    }

    try {
        NoC noc;
        std::cout << "Loading configuration from: " << config_path << std::endl;
        noc.load_from_json(config_path);

        std::cout << "Calculating flows..." << std::endl;
        auto loads = noc.calculate_edge_loads();

        NoC::print_edge_loads(loads);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
