#include "configer.h"
#include "packer.h"
#include "plotter.h"
#include "yaml-cpp/yaml.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.yaml>" << std::endl;
        return 1;
    }

    try {
        std::cout << "Loading YAML file..." << std::endl;
        YAML::Node config = YAML::LoadFile(argv[1]);
        if (!config) {
            std::cerr << "Failed to load YAML file: " << argv[1] << std::endl;
            return 1;
        }
        auto [refParam, resParams, optParam] = Configer::yaml2params(config);

        std::cout << "Processing data..." << std::endl;
        auto [refPack, resPacks] = Packer::param2pack(refParam, resParams);

        std::cout << "Generating plots..." << std::endl;
        Plotter::plot(refPack, resPacks, optParam);

        std::cout << "Saving data ..." << std::endl;
        Plotter::save_df(refPack, resPacks, optParam);

        std::cout << "Completed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
