//
// Created by bartek on 02.07.22.
//

#ifndef CLANG_INCLUDE_GRAPH_CONFIG_H
#define CLANG_INCLUDE_GRAPH_CONFIG_H

#include "util.h"

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace clang_include_graph {

enum class printer_t { topological_sort, tree, unknown };

struct config_t {
    bool verbose{false};
    boost::optional<std::string> compilation_database_directory;
    boost::optional<std::string> translation_unit;
    boost::optional<std::string> relative_to;
    bool filenames_only;
    printer_t printer{printer_t::topological_sort};

    void init(boost::program_options::variables_map &vm)
    {
        if (vm.count("verbose") == 1)
            verbose = true;

        compilation_database_directory = util::to_absolute_path(".");

        if (vm.count("compilation-database-dir") == 1) {
            compilation_database_directory = util::to_absolute_path(
                vm["compilation-database-dir"].as<std::string>());
        }

        if (!compilation_database_directory) {
            std::cerr
                << "ERROR: Cannot find compilation database - aborting...";
            exit(-1);
        }

        if (vm.count("relative-to") == 1) {
            relative_to =
                util::to_absolute_path(vm["relative-to"].as<std::string>());
        }

        if (vm.count("names-only") == 1) {
            filenames_only = true;
        }

        if (relative_to.has_value() && filenames_only) {
            std::cerr << "ERROR: --relative-to and --names-only cannot be enabled "
                         "at the same time"
                      << " - aborting...";
            exit(-1);
        }

        if (vm.count("translation-unit") == 1) {
            translation_unit = util::to_absolute_path(
                vm["translation-unit"].as<std::string>());
            if (!translation_unit) {
                std::cerr << "ERROR: Cannot find translation unit source at "
                          << vm["translation-unit"].as<std::string>()
                          << " - aborting...";
                exit(-1);
            }
        }

        if (vm.count("tree")) {
            printer = printer_t::tree;
        }
    }
};

} // namespace clang_include_graph

#endif // CLANG_INCLUDE_GRAPH_CONFIG_H
