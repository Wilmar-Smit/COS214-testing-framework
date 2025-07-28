#include "include/testfw/testfw.hpp"
#include <memory>
#include <getopt.h>
#include <fstream>

int main(int argc, char* argv[]) {
    // CLI argument parsing (basic academic workflow)
    std::string configFile = "";
    std::string outputFormat = "console";
    bool listOnly = false;
    bool help = false;
    bool memoryCheck = false;
    std::string tag = "";
    int opt;
    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"format", required_argument, 0, 'f'},
        {"list", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {"memory", no_argument, 0, 'm'},
        {"tag", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };
    while ((opt = getopt_long(argc, argv, "c:f:lht:m", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'c': configFile = optarg; break;
            case 'f': outputFormat = optarg; break;
            case 'l': listOnly = true; break;
            case 'h': help = true; break;
            case 'm': memoryCheck = true; break;
            case 't': tag = optarg; break;
            default: help = true; break;
        }
    }
    if (help) {
        HelpCommand helpCmd;
        helpCmd.execute({});
        return 0;
    }
    if (listOnly) {
        ListTestsCommand listCmd;
        return listCmd.execute({});
    }
    Config config;
    if (!configFile.empty()) {
        config = ConfigParser::parseFromFile(configFile);
    }
    if (memoryCheck) {
        config.execution.memory_check = true;
    }
    std::unique_ptr<OutputStrategy> output;
    if (outputFormat == "console") {
        output = std::make_unique<ConsoleOutputStrategy>();
    } else if (outputFormat == "json") {
        output = std::make_unique<JsonOutputStrategy>();
    } else {
        // Future: add Markdown/XML output
        output = std::make_unique<ConsoleOutputStrategy>();
    }
    auto observer = std::make_shared<ConsoleTestObserver>();
    TestRunner runner(config, std::move(output), {observer});
    if (!tag.empty()) {
        runner.runWithTag(tag);
    } else {
        runner.runAll();
    }
    return 0;
}
