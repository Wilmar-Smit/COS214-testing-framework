#include "CLI.h"
#include <iostream>
#include <sstream>

// This demo simulates CLI usage and shows the new commandRegistry-based logic in action.
// For a real test, run this and try various commands, typos, and menu options.

int main() {
    CLI& cli = CLI::getInstance();
    std::cout << "\n--- CLI Demo ---\n";
    std::cout << "Type commands like 'help', 'status', 'hlp', 'switch', or menu numbers (0-4)." << std::endl;
    std::cout << "Try typos to see suggestions. Type 'quit' to exit.\n" << std::endl;
    cli.run();
    std::cout << "\n--- CLI Demo Ended ---\n";
    return 0;
}

