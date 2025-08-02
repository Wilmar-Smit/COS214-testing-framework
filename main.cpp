#include "Logger.h"
#include "CLI.h"

int main() {
    Logger* logger = Logger::getInstance("cli.log");
    logger->info("Starting CLI application");
    
    CLI& cli = CLI::getInstance();
    cli.run();
    
    logger->info("CLI application terminated");
    return 0;
}
