#include "CLIConstants.h"

namespace CLIConstants {
    // Prompts
    const std::string TEXT_MODE_PROMPT = "> ";
    const std::string NUMBER_MODE_PROMPT = "Enter option number> ";
    
    // Messages
    const std::string WELCOME_MESSAGE = "CLI Manager v1.0\nType 'help' for commands";
    const std::string EXIT_MESSAGE = "Goodbye!";
    const std::string MODE_SWITCH_SUCCESS = "Mode switched successfully.";
    const std::string INVALID_COMMAND = "Invalid command.";
    
    // Menu
    const std::string MENU_HEADER = "Menu Options";
    const std::string HELP_HEADER = "Help";
    const std::string SEPARATOR_LINE = "────────────────────────────────────────";
    
    // Error Messages
    const std::string UNKNOWN_COMMAND_ERROR = "Unknown command.";
    const std::string INVALID_NUMBER_ERROR = "Invalid number.";
    const std::string EMERGENCY_SHUTDOWN = "Emergency shutdown.";
}