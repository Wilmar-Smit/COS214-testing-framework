#ifndef CLICONSTANTS_H
#define CLICONSTANTS_H

#include <string>

namespace CLIConstants {
    // Prompts
    extern const std::string TEXT_MODE_PROMPT;
    extern const std::string NUMBER_MODE_PROMPT;
    
    // Messages
    extern const std::string WELCOME_MESSAGE;
    extern const std::string EXIT_MESSAGE;
    extern const std::string MODE_SWITCH_SUCCESS;
    extern const std::string INVALID_COMMAND;
    
    // Menu
    extern const std::string MENU_HEADER;
    extern const std::string HELP_HEADER;
    extern const std::string SEPARATOR_LINE;
    
    // Error Messages
    extern const std::string UNKNOWN_COMMAND_ERROR;
    extern const std::string INVALID_NUMBER_ERROR;
    extern const std::string EMERGENCY_SHUTDOWN;

    // Magic Numbers
    const int MENU_SWITCH_MODE = 0;
    const int MENU_HELP = 1;
    const int MENU_STATUS = 2;
    const int MENU_CLEAR = 3;
    const int MENU_QUIT = 4;
    const int MAX_MENU_OPTION = 4;
}

#endif // CLICONSTANTS_H