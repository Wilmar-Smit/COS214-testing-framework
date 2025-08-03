//
// CLI.cpp - Singleton CLI Manager Class Implementation
// Created by Michael Tomlinson on 2025/08/01
//
// WHAT IS THIS FILE?
// This file implements a command-line interface (CLI) manager using a singleton pattern.
// It is designed to be easily extensible for new commands through a centralized command registry.
// All output is channeled through the Logger class for consistent formatting and logging.
//
// HOW TO ADD A NEW COMMAND:
// 1.  **Add a CommandType Enum:**
//     - Open `CLI.h` and add a new value to the `CommandType` enum (e.g., `NEW_COMMAND`).
//
// 2.  **Register the Command:**
//     - Open `CLI.cpp` and add a new entry to the `COMMAND_REGISTRY` array.
//     - Example:
//       { CLI::CommandType::NEW_COMMAND, "new", {"n", "newcmd"}, "Description of new command", "Help text for new command", 5 },
//     - `menuPosition` determines its place in number mode. Use -1 if it should not appear in the menu.
//
// 3.  **Implement Command Logic:**
//     - In `CLI.cpp`, go to the `executeCommand` method.
//     - Add a new `case` to the `switch` statement for your new `CommandType`.
//     - Example:
//       case CommandType::NEW_COMMAND:
//           logger->info("Executing the new command!");
//           // Add your command's logic here
//           break;
//
// That's it! The command will be automatically available in text mode, number mode (if menuPosition is set),
// and included in the help and command suggestion systems.
//

#include "CLI.h"
#include "Logger.h"
#include "CLIConstants.h"
#include <cstdlib>  // For getenv()
#include <algorithm> // For std::sort and std::min

CLI& CLI::getInstance() {
    static CLI instance; // Static local variable ensures thread-safe singleton
    return instance;
}

CLI::CLI()
    : currentMode(Mode::NUMBER_MODE),  // Default to beginner-friendly mode
      running(false),
      colorsEnabled(true),
      prompt(CLIConstants::TEXT_MODE_PROMPT),
      welcomeMessage(CLIConstants::WELCOME_MESSAGE),
      exitMessage(CLIConstants::EXIT_MESSAGE),
      defaultMode(Mode::NUMBER_MODE),  // Default startup mode
      currentColorScheme(ColorScheme::DEFAULT)
{
    logger = Logger::getInstance("cli.log");
    initializeCommandRegistry();
    
    initializeColorScheme();
    
    detectColorSupport();
    
    currentMode = defaultMode;
}

static const CLI::CommandInfo COMMAND_REGISTRY[] = {
    {CLI::CommandType::HELP, "help", {"h"}, "Display help information", "Shows available commands and usage", 1},
    {CLI::CommandType::QUIT, "quit", {"exit", "q"}, "Exit the CLI", "Gracefully shutdown the application", 4},
    {CLI::CommandType::SWITCH_MODE, "switch", {"mode", "menu", "m"}, "Switch input mode", "Toggle between text and number mode", 0},
    {CLI::CommandType::CLEAR, "clear", {"cls"}, "Clear screen", "Clear the terminal screen", 3},
    {CLI::CommandType::STATUS, "status", {"info"}, "Show system status", "Display current CLI state and settings", 2}
};

void CLI::initializeCommandRegistry() {
    for (const auto& cmd : COMMAND_REGISTRY) {
        commandRegistry.push_back(cmd);
    }
}

const CLI::CommandInfo* CLI::findCommandByName(const std::string& name) const {
    for (const auto& cmd : commandRegistry) {
        if (toLowerCase(name) == toLowerCase(cmd.primaryName)) {
            return &cmd;
        }
        for (const auto& alias : cmd.aliases) {
            if (toLowerCase(name) == toLowerCase(alias)) {
                return &cmd;
            }
        }
    }
    return nullptr;
}

const CLI::CommandInfo* CLI::findCommandByMenuPosition(int position) const {
    for (const auto& cmd : commandRegistry) {
        if (cmd.menuPosition == position) {
            return &cmd;
        }
    }
    return nullptr;
}

std::vector<CLI::CommandInfo> CLI::getMenuCommands() const {
    std::vector<CLI::CommandInfo> menuCommands;
    for (const auto& cmd : commandRegistry) {
        if (cmd.menuPosition != -1) {
            menuCommands.push_back(cmd);
        }
    }
    std::sort(menuCommands.begin(), menuCommands.end(), [](const CLI::CommandInfo& a, const CLI::CommandInfo& b) {
        return a.menuPosition < b.menuPosition;
    });
    return menuCommands;
}

const std::vector<CLI::CommandInfo>& CLI::getCommandRegistry() const {
    return commandRegistry;
}

CLI::CommandType CLI::parseCommand(const std::string& input) {
    std::string sanitized = sanitizeInput(input);
    if (sanitized.empty()) {
        return CommandType::UNKNOWN;
    }

    std::string commandName;
    size_t spacePos = sanitized.find(' ');
    if (spacePos != std::string::npos) {
        commandName = sanitized.substr(0, spacePos);
    } else {
        commandName = sanitized;
    }

    const CommandInfo* command = findCommandByName(commandName);
    if (command) {
        return command->type;
    }

    return CommandType::UNKNOWN;
}







// This might need to be disabled - not sure how Fitchfork will handle this
bool CLI::detectColorSupport() {
    const char* term = std::getenv("TERM");
    const char* colorterm = std::getenv("COLORTERM");
    
    if (!term) {
        colorsEnabled = false;
        return false;
    }
    
    std::string termStr(term);
    
    bool hasColorSupport = (termStr.find("xterm") != std::string::npos ||
                           termStr.find("color") != std::string::npos ||
                           termStr.find("ansi") != std::string::npos ||
                           termStr.find("screen") != std::string::npos ||
                           termStr.find("tmux") != std::string::npos ||
                           colorterm != nullptr);
    
    // Additional check for Windows terminals
    #ifdef _WIN32
        const char* wt_session = std::getenv("WT_SESSION");
        const char* wt_profile_id = std::getenv("WT_PROFILE_ID");
        if (wt_session || wt_profile_id) {
            hasColorSupport = true;
        }
    #endif
    
    colorsEnabled = hasColorSupport;
    
    if (!hasColorSupport) {
        logger->info("Color support not detected, using plain text mode");
    }
    
    return hasColorSupport;
}

void CLI::run() {
    try {
        running = true;
        
        if (!validateInitialState()) {
            logger->error("CLI initialization failed. Cannot start.");
            return;
        }
        
        logger->info(welcomeMessage);
        
        std::string input;
        int consecutiveErrors = 0;
        const int maxConsecutiveErrors = 5;
        
        while (running) {
            try {
                // Might need to adjust this so the menu doesn't display in text mode without being requested
                if (currentMode == Mode::NUMBER_MODE) {
                    displayMenu();
                    displayNumberModePrompt();
                } else {
                    displayTextModePrompt();
                }
                
                if (std::getline(std::cin, input)) {
                    bool commandSuccess = processCommand(input);
                    
                    if (commandSuccess) {
                        consecutiveErrors = 0;
                    } else {
                        consecutiveErrors++;
                        
                        if (consecutiveErrors >= 3) {
                            logger->warning("Multiple errors detected. Consider switching modes or typing 'help'");
                        }

                        // Stop if everything is failing - the OSOF Protocol
                        if (consecutiveErrors >= maxConsecutiveErrors) {
                            logger->error("Too many consecutive errors. Initiating emergency shutdown...");
                            handleEmergencyShutdown();
                            break;
                        }
                    }
                } else {
                    // This is what AI recommended - basically stop listening for a bit and see if that helps
                    if (std::cin.eof()) {
                        logger->info("\nEnd of input detected (Ctrl+D). Exiting gracefully...");
                    } else if (std::cin.fail()) {
                        logger->warning("\nInput stream error detected. Attempting recovery...");
                        
                        std::cin.clear();
                        std::cin.ignore(1000, '\n');
                        
                        consecutiveErrors++;
                        if (consecutiveErrors >= maxConsecutiveErrors) {
                            logger->error("Cannot recover from input errors. Exiting...");
                            break;
                        }
                        continue;
                    } else {
                        logger->error("\nUnknown input error. Exiting...");
                    }
                    break;
                }
            } catch (const std::exception& e) {
                logger->error("Error during command processing: " + std::string(e.what()));
                consecutiveErrors++;
                
                if (consecutiveErrors >= maxConsecutiveErrors) {
                    logger->error("Too many errors. Shutting down for safety...");
                    break;
                }
                
                logger->info("Attempting to recover and continue...");
            } catch (...) {
                logger->error("Unknown error occurred during command processing");
                consecutiveErrors++;
                
                if (consecutiveErrors >= maxConsecutiveErrors) {
                    logger->error("Critical error. Emergency shutdown...");
                    break;
                }
            }
        }
        
        if (running) {
            running = false;
        }
        
    } catch (const std::exception& e) {
        logger->error("Critical error in CLI run loop: " + std::string(e.what()));
        handleEmergencyShutdown();
    } catch (...) {
        logger->error("Unknown critical error in CLI run loop");
        handleEmergencyShutdown();
    }
}

bool CLI::processCommand(const std::string& input) {
    try {
        if (!validateInput(input)) {
            return false;
        }
        
        CommandType commandType;
        std::vector<std::string> arguments;
        
        if (currentMode == Mode::NUMBER_MODE) {
            int menuOption;
            if (!validateNumberInput(input, menuOption)) {
                handleInvalidNumberInput(input);
                return false;
            }
            
            commandType = convertMenuOptionToCommand(menuOption);
            if (commandType == CommandType::UNKNOWN) {
                logger->error("Internal error: Invalid menu option conversion");
                return false;
            }
            
            arguments.clear();
            
            if (!provideMenuSelectionFeedback(menuOption)) {
                logger->warning("Warning: Could not provide selection feedback");
            }
            
        } else {
            try {
                commandType = parseCommand(input);
            } catch (const std::exception& e) {
                logger->error("Error parsing command: " + std::string(e.what()));
                return false;
            }
            
            try {
                arguments = parseArguments(input);
            } catch (const std::exception& e) {
                logger->error("Error parsing arguments: " + std::string(e.what()));
                logger->warning("Command will be executed without arguments");
                arguments.clear();
            }
            
            if (commandType == CommandType::UNKNOWN) {
                std::string commandName = extractCommandName(input, arguments);
                handleUnknownCommand(commandName);
                return false;
            }
            
            if (!validateCommandArguments(commandType, arguments)) {
                return false;
            }
        }
        
        try {
            executeCommand(commandType, arguments);
            return true;
        } catch (const std::exception& e) {
            logger->error("Error executing command: " + std::string(e.what()));
            return false;
        }
        
    } catch (const std::exception& e) {
        logger->error("Unexpected error processing command: " + std::string(e.what()));
        return false;
    } catch (...) {
        logger->error("Unknown error occurred while processing command");
        return false;
    }
}

void CLI::executeCommand(CommandType commandType, const std::vector<std::string>& arguments) {
    (void)arguments; // This tells the compiler I am intentionally not using arguments here. 
    
    try {
        if (!validateExecutionState()) {
            logger->error("CLI is not in a valid state for command execution");
            return;
        }
        
        switch (commandType) {
            case CommandType::HELP:
                try {
                    displayHelp();
                } catch (const std::exception& e) {
                    logger->error("Error displaying help: " + std::string(e.what()));
                    logger->info("Basic help: Available commands are help, quit, switch, clear, status");
                }
                break;
                
            case CommandType::QUIT:
                try {
                    logger->info("Initiating graceful shutdown...");
                    shutdown();
                } catch (const std::exception& e) {
                    logger->error("Error during shutdown: " + std::string(e.what()));
                    running = false;
                }
                break;
                
            case CommandType::SWITCH_MODE:
                try {
                    if (currentMode == Mode::TEXT_MODE) {
                        logger->info("Switching from text mode to number mode...");
                        switchToNumberMode();
                    } else {
                        logger->info("Switching from number mode to text mode...");
                        switchToTextMode();
                    }
                } catch (const std::exception& e) {
                    logger->error("Error switching modes: " + std::string(e.what()));
                    logger->warning("Mode switch failed. Current mode preserved.");
                }
                break;
                
            case CommandType::CLEAR:
                try {
                    clearScreen();
                    logger->info("Screen cleared.");
                } catch (const std::exception& e) {
                    logger->error("Error clearing screen: " + std::string(e.what()));
                    logger->warning("Screen clearing failed. You may need to clear manually.");
                }
                break;
                
            case CommandType::STATUS: // This can be adapted based on the project
                try {
                    displayCurrentMode();
                    logger->info("Colors enabled: " + std::string(colorsEnabled ? "Yes" : "No"));
                    logger->info("Running: " + std::string(running ? "Yes" : "No"));
                    logger->info("Terminal color support: " + std::string(supportsColors() ? "Yes" : "No"));
                    
                    std::string colorScheme;
                    switch (currentColorScheme) {
                        case ColorScheme::DEFAULT: colorScheme = "Default"; break;
                        case ColorScheme::DARK: colorScheme = "Dark"; break;
                        case ColorScheme::MATRIX: colorScheme = "Matrix"; break;
                        case ColorScheme::RETRO: colorScheme = "Retro"; break;
                        case ColorScheme::HIGH_CONTRAST: colorScheme = "High Contrast"; break;
                        default: colorScheme = "Unknown"; break;
                    }
                    logger->info("Color scheme: " + colorScheme);
                    
                } catch (const std::exception& e) {
                    logger->error("Error displaying status: " + std::string(e.what()));
                    logger->info("Basic status: CLI is running in " + 
                          std::string(currentMode == Mode::TEXT_MODE ? "text" : "number") + " mode");
                }
                break;
                
            case CommandType::UNKNOWN:
            default:
                // Should never get here - unknown commands should be picked up earlier
                logger->error("Error: Unknown command encountered during execution");
                logger->warning("This is an internal error. Please report this issue.");
                
                if (currentMode == Mode::TEXT_MODE) {
                    logger->info("Try typing 'help' to see available commands");
                } else {
                    logger->info("Try selecting option 1 from the menu to see help");
                }
                break;
        }
        
    } catch (const std::exception& e) {
        logger->error("Critical error during command execution: " + std::string(e.what()));
        logger->warning("Command execution failed. CLI state may be unstable.");
        
        try {
            if (commandType == CommandType::QUIT) {
                running = false;
                logger->warning("Forced shutdown due to quit command failure");
            }
        } catch (...) {
            logger->warning("Recovery attempt failed. Continuing with caution.");
        }
        
    } catch (...) {
        logger->error("Unknown critical error during command execution");
        logger->warning("CLI may be in an unstable state. Consider restarting.");
    }
}

void CLI::displayMenu() const {
    if (currentMode != Mode::NUMBER_MODE) {
        return;
    }

    printBanner("Menu Options");

    std::vector<CommandInfo> menuCommands = getMenuCommands();

    for (const auto& cmd : menuCommands) {
        print(std::to_string(cmd.menuPosition) + ". " + cmd.description, MessageType::COMMAND);
    }

    printSeparator();
}

void CLI::displayHelp() const {
    if (currentMode == Mode::TEXT_MODE) {
        printBanner("Text Mode Commands");
        print("Commands are case-insensitive and support aliases:", MessageType::INFO);
        printSeparator();

        for (const auto& cmd : commandRegistry) {
            std::string aliases;
            if (!cmd.aliases.empty()) {
                aliases = " (";
                for (size_t i = 0; i < cmd.aliases.size(); ++i) {
                    aliases += cmd.aliases[i] + (i < cmd.aliases.size() - 1 ? ", " : "");
                }
                aliases += ")";
            }
            print(cmd.primaryName + aliases + " - " + cmd.helpText, MessageType::DESCRIPTION);
        }

        printSeparator();
        print("Tip: You can use partial commands and get suggestions for typos!", MessageType::INFO);
    } else {
        printBanner("Number Mode Help");
        print("In number mode, select options by entering the corresponding number:", MessageType::INFO);
        printSeparator();

        std::vector<CommandInfo> menuCommands = getMenuCommands();

        for (const auto& cmd : menuCommands) {
            print(std::to_string(cmd.menuPosition) + " - " + cmd.helpText, MessageType::DESCRIPTION);
        }

        printSeparator();
        print("Tip: Text mode allows direct command entry with aliases and suggestions!", MessageType::INFO);
    }
}

void CLI::shutdown() {
    running = false;
    print(exitMessage, MessageType::INFO);
}

void CLI::switchToTextMode() {
    if (currentMode == Mode::TEXT_MODE) {
        print("Already in text mode.", MessageType::INFO);
        return;
    }
    
    currentMode = Mode::TEXT_MODE;
    print("Switched to text mode.", MessageType::SUCCESS);
    print("You can now enter commands directly (e.g., 'help', 'quit').", MessageType::INFO);
    print("Commands are case-insensitive and support aliases.", MessageType::INFO);
}



void CLI::switchToNumberMode() {
    if (currentMode == Mode::NUMBER_MODE) {
        logger->info("Already in number mode.");
        return;
    }
    
    currentMode = Mode::NUMBER_MODE;
    logger->info(CLIConstants::MODE_SWITCH_SUCCESS);
    logger->info("A numbered menu will be displayed before each prompt.");
    logger->info("Enter the number corresponding to your choice.");
}







CLI::Mode CLI::getCurrentMode() const {
    return currentMode;
}

void CLI::displayCurrentMode() const {
    std::string modeStr = (currentMode == Mode::TEXT_MODE) ? "Text Mode" : "Number Mode";
    std::string description = (currentMode == Mode::TEXT_MODE) ? 
        "Enter commands directly (case-insensitive, supports aliases)" :
        "Select options from numbered menu";
    
    print("Current mode: " + modeStr, MessageType::INFO);
    print("Description: " + description, MessageType::DESCRIPTION);
}

// Configuration methods - these allow another class to configure the CLI - reduces how much is hard coded in this template

void CLI::setPrompt(const std::string& newPrompt) {
    if (!validatePrompt(newPrompt)) {
        print("Invalid prompt: Prompt cannot be empty or contain only whitespace", MessageType::ERROR);
        return;
    }
    
    prompt = newPrompt;
    print("Prompt updated successfully", MessageType::SUCCESS);
}

void CLI::setWelcomeMessage(const std::string& message) {
    if (!validateMessage(message)) {
        print("Invalid welcome message: Message cannot exceed 1000 characters", MessageType::ERROR);
        return;
    }
    
    welcomeMessage = message;
    print("Welcome message updated successfully", MessageType::SUCCESS);
}

void CLI::setExitMessage(const std::string& message) {
    if (!validateMessage(message)) {
        print("Invalid exit message: Message cannot exceed 1000 characters", MessageType::ERROR);
        return;
    }
    
    exitMessage = message;
    print("Exit message updated successfully", MessageType::SUCCESS);
}

// This is just generic from AI - but basically check if the stuff given to us isn't stupid
bool CLI::validatePrompt(const std::string& prompt) const {
    if (prompt.empty()) {
        return false;
    }
    
    bool hasNonWhitespace = false;
    for (char c : prompt) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            hasNonWhitespace = true;
            break;
        }
    }
    
    if (!hasNonWhitespace) {
        return false;
    }
    
    if (prompt.length() > 50) {
        return false;
    }
    
    return true;
}

bool CLI::validateMessage(const std::string& message) const {
    if (message.length() > 1000) {
        return false;
    }
    
    return true;
}

CLI::Mode CLI::getDefaultMode() const {
    return defaultMode;
}

CLI::ColorScheme CLI::getColorScheme() const {
    return currentColorScheme;
}

void CLI::enableColors(bool enable) {
    if (enable && !detectColorSupport()) {
        print("Warning: Terminal may not support ANSI colors, but colors have been enabled", MessageType::WARNING);
    }
    
    colorsEnabled = enable;
    
    if (enable) {
        print("Colors enabled", MessageType::SUCCESS);
    } else {
        std::cout << "Colors disabled" << std::endl;
    }
}

// More configuration methods - reduce hardcoding and allow for dynamic changes
void CLI::setDefaultMode(Mode mode) {
    defaultMode = mode;
    
    std::string modeStr = (mode == Mode::TEXT_MODE) ? "Text Mode" : "Number Mode";
    print("Default startup mode set to: " + modeStr, MessageType::SUCCESS);
    print("This will take effect on next CLI restart", MessageType::INFO);
}

void CLI::setColorScheme(ColorScheme scheme) {
    currentColorScheme = scheme;
    applyColorScheme(scheme);
    
    std::string schemeName;
    switch (scheme) {
        case ColorScheme::DEFAULT:
            schemeName = "Default";
            break;
        case ColorScheme::DARK:
            schemeName = "Dark";
            break;
        case ColorScheme::MATRIX:
            schemeName = "Matrix";
            break;
        case ColorScheme::RETRO:
            schemeName = "Retro";
            break;
        case ColorScheme::HIGH_CONTRAST:
            schemeName = "High Contrast";
            break;
        default:
            schemeName = "Unknown";
            break;
    }
    
    print("Color scheme changed to: " + schemeName, MessageType::SUCCESS);
}

void CLI::print(const std::string& message, MessageType type) const {
    switch (type) {
        case MessageType::PROMPT:
        case MessageType::COMMAND:
        case MessageType::DESCRIPTION:
            logger->printRaw(message);
            break;
        case MessageType::INFO:
        case MessageType::SUCCESS:
            logger->printRaw(message);
            break;
        case MessageType::WARNING:
            logger->warning(message);
            break;
        case MessageType::ERROR:
            logger->error(message);
            break;
    }
}

void CLI::printColored(const std::string& message, const std::string& color) const {
    (void)color; // color is no longer used
    logger->info(message);
}

// Basic banner - looks pretty and professional - might make this more customizable later
void CLI::printBanner(const std::string& text) const {
    if (text.empty()) {
        return;
    }
    size_t textLength = text.length();
    size_t bannerWidth = textLength + 8;
    if (bannerWidth < 20) {
        bannerWidth = 20;
    }
    std::string topBorder = "╔";
    for (size_t i = 0; i < bannerWidth - 2; ++i) {
        topBorder += "═";
    }
    topBorder += "╗";
    std::string bottomBorder = "╚";
    for (size_t i = 0; i < bannerWidth - 2; ++i) {
        bottomBorder += "═";
    }
    bottomBorder += "╝";
    std::string middleLine = "║";
    size_t padding = (bannerWidth - 2 - textLength) / 2;
    size_t extraPadding = (bannerWidth - 2 - textLength) % 2;
    for (size_t i = 0; i < padding; ++i) {
        middleLine += " ";
    }
    middleLine += text;
    for (size_t i = 0; i < padding + extraPadding; ++i) {
        middleLine += " ";
    }
    middleLine += "║";
    // Use printRaw for banner to avoid timestamps
    logger->printRaw(topBorder);
    logger->printRaw(middleLine);
    logger->printRaw(bottomBorder);
}

void CLI::printSeparator() const {
    logger->info("────────────────────────────────────────");
}

// Utility methods
void CLI::clearScreen() const {
    bool clearSuccess = false;
    
    #ifdef _WIN32
        int result = std::system("cls");
        if (result == 0) {
            clearSuccess = true;
        } else {
            // Method 2: Try Windows cmd cls command
            result = std::system("cmd /c cls");
            if (result == 0) {
                clearSuccess = true;
            }
        }
        
    #else
        // Unix-like platforms (Linux, macOS, etc.)
        
        int result = std::system("clear");
        if (result == 0) {
            clearSuccess = true;
        } else {
            result = std::system("tput clear");
            if (result == 0) {
                clearSuccess = true;
            }
        }
        
    #endif
    
    if (!clearSuccess) {
        if (colorsEnabled && supportsColors()) {
            // ANSI escape sequence: ESC[2J (clear entire screen) + ESC[H (move cursor to home)
            std::cout << "\033[2J\033[H" << std::flush;
            clearSuccess = true;
        } else {
            // Final fallback: print multiple newlines to simulate clearing
            for (int i = 0; i < 50; ++i) {
                std::cout << "\n";
            }
            std::cout << std::flush;
            
            if (colorsEnabled) {
                print("Screen clearing failed, used newlines as fallback", MessageType::WARNING);
            } else {
                std::cout << "Screen clearing failed, used newlines as fallback" << std::endl;
            }
        }
    }
}

bool CLI::isRunning() const {
    return running;
}

bool CLI::supportsColors() const {
    return colorsEnabled;
}


std::string CLI::toLowerCase(const std::string& str) const {
    std::string result = str;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            // Pretty neat code - just use chars and int to correctly convert to lowercase
            c = c + ('a' - 'A');
        }
    }
    return result;
}

// Clean input to make it safer to work with
std::string CLI::sanitizeInput(const std::string& input) const {
    std::string sanitized = input;
    
    // Trim leading whitespace
    size_t start = 0;
    while (start < sanitized.length() && (sanitized[start] == ' ' || sanitized[start] == '\t' || sanitized[start] == '\n' || sanitized[start] == '\r')) {
        start++;
    }
    
    // Trim trailing whitespace
    size_t end = sanitized.length();
    while (end > start && (sanitized[end - 1] == ' ' || sanitized[end - 1] == '\t' || sanitized[end - 1] == '\n' || sanitized[end - 1] == '\r')) {
        end--;
    }
    
    // Extract trimmed substring
    if (start < end) {
        sanitized = sanitized.substr(start, end - start);
    } else {
        sanitized = "";
    }
    
    // Validate input length
    if (sanitized.length() > 1000) {
        sanitized = sanitized.substr(0, 1000);
    }
    
    return sanitized;
}

// Core function - turns input into a command type


// In theory this should allow for super advanced commands without needing to change the CLI class
std::vector<std::string> CLI::parseArguments(const std::string& input) {
    std::vector<std::string> arguments;
    
    try {
        std::string sanitized;
        try {
            sanitized = sanitizeInput(input);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error sanitizing input: " + std::string(e.what()));
        }
        
        if (sanitized.empty()) {
            return arguments;
        }

        std::string current = "";
        bool inQuotes = false;
        bool escapeNext = false;
        
        try {
            for (size_t i = 0; i < sanitized.length(); i++) {
                char c = sanitized[i];
                
                if (escapeNext) {
                    current += c;
                    escapeNext = false;
                } else if (c == '\\') {
                    escapeNext = true;
                } else if (c == '"' && !inQuotes) {
                    inQuotes = true;
                } else if (c == '"' && inQuotes) {
                    inQuotes = false;
                } else if ((c == ' ' || c == '\t') && !inQuotes) {
                    if (!current.empty()) {
                        arguments.push_back(current);
                        current = "";
                    }
                } else {
                    current += c;
                }

                if (arguments.size() > 1000) {
                    throw std::runtime_error("Too many arguments detected (limit: 1000)");
                }
            }

            if (inQuotes) {
                throw std::runtime_error("Unclosed quote in input");
            }
            if (escapeNext) {
                throw std::runtime_error("Trailing escape character in input");
            }
            if (!current.empty()) {
                arguments.push_back(current);
            }
            
        } catch (const std::exception& e) {
            throw std::runtime_error("Error during token parsing: " + std::string(e.what()));
        }
        for (const std::string& arg : arguments) {
            if (arg.length() > 1000) {
                throw std::runtime_error("Argument too long (limit: 1000 characters)");
            }
        }
        
        return arguments;
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Argument parsing failed: " + std::string(e.what()));
    } catch (...) {
        throw std::runtime_error("Unknown error during argument parsing");
    }
}

// Text mode specific methods
void CLI::displayTextModePrompt() const {
    if (currentMode != Mode::TEXT_MODE) {
        return;
    }

    std::string modeIndicator = "[TEXT] ";
    if (colorsEnabled) {
        std::cout << HGRN << modeIndicator << CRESET << BLU << prompt << CRESET;
    } else {
        std::cout << modeIndicator << prompt;
    }
    std::cout.flush(); // Ensure prompt is displayed immediately
}

// Super interesting algorithm - Levenshtein distance algorithm - basically it is simple autocomplete
int CLI::calculateLevenshteinDistance(const std::string& s1, const std::string& s2) const {
    const size_t len1 = s1.size();
    const size_t len2 = s2.size();
    
    // Create a matrix to store distances
    std::vector<std::vector<int>> matrix(len1 + 1, std::vector<int>(len2 + 1));
    
    // Initialize first row and column
    for (size_t i = 0; i <= len1; ++i) {
        matrix[i][0] = static_cast<int>(i);
    }
    for (size_t j = 0; j <= len2; ++j) {
        matrix[0][j] = static_cast<int>(j);
    }
    
    // Fill the matrix
    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            
            matrix[i][j] = std::min({
                matrix[i - 1][j] + 1,      // deletion
                matrix[i][j - 1] + 1,      // insertion
                matrix[i - 1][j - 1] + cost // substitution
            });
        }
    }
    
    return matrix[len1][len2];
}

// Get similar commands based on Levenshtein distance
std::vector<std::string> CLI::getSimilarCommands(const std::string& input) const {
    std::vector<std::string> suggestions;
    std::string lowerInput = toLowerCase(input);
    int maxDistance = std::max(1, static_cast<int>(input.length()) / 3);
    for (const auto& cmd : commandRegistry) {
        // Check primary name
        int distance = calculateLevenshteinDistance(lowerInput, toLowerCase(cmd.primaryName));
        if (distance <= maxDistance && distance > 0) {
            if (std::find(suggestions.begin(), suggestions.end(), cmd.primaryName) == suggestions.end()) {
                suggestions.push_back(cmd.primaryName);
            }
        }
        // Check aliases
        for (const auto& alias : cmd.aliases) {
            int aliasDistance = calculateLevenshteinDistance(lowerInput, toLowerCase(alias));
            if (aliasDistance <= maxDistance && aliasDistance > 0) {
                if (std::find(suggestions.begin(), suggestions.end(), alias) == suggestions.end()) {
                    suggestions.push_back(alias);
                }
            }
        }
    }
    return suggestions;
}

void CLI::suggestCommands(const std::string& invalidCommand) const {
    if (currentMode != Mode::TEXT_MODE) {
        return;
    }
    
    try {
        std::string trimmed = sanitizeInput(invalidCommand);
        
        if (trimmed.empty()) {
            print("Empty command entered. Please type a command or 'help' for assistance.", MessageType::INFO);
            return;
        }

        std::vector<std::string> suggestions = getSimilarCommands(trimmed);
        
        if (!suggestions.empty()) {
            print("Did you mean:", MessageType::INFO);
            for (const std::string& suggestion : suggestions) {
                print("  " + suggestion, MessageType::COMMAND);
            }
        } else {
            print("No similar commands found for '" + trimmed + "'", MessageType::INFO);
            analyzeCommandMistakes(trimmed);
        }

        print("Type 'help' to see all available commands", MessageType::INFO);

        if (trimmed.length() == 1 && trimmed[0] >= '0' && trimmed[0] <= '9') {
            print("Tip: Numbers are used in menu mode. Type 'switch' to change modes.", MessageType::INFO);
        }
        
    } catch (const std::exception& e) {
        print("Error generating command suggestions: " + std::string(e.what()), MessageType::ERROR);
        print("Type 'help' to see available commands", MessageType::INFO);
    } catch (...) {
        print("Unknown error generating suggestions", MessageType::ERROR);
        print("Type 'help' to see available commands", MessageType::INFO);
    }
}

// Number mode specific methods

void CLI::displayNumberModePrompt() const {
    if (currentMode != Mode::NUMBER_MODE) {
        return;
    }

    std::string modeIndicator = "[MENU] ";
    if (colorsEnabled) {
        std::cout << HYEL << modeIndicator << CRESET << BLU << "Enter option number" << prompt << CRESET;
    } else {
        std::cout << modeIndicator << "Enter option number" << prompt;
    }
    std::cout.flush(); // Ensure prompt is displayed immediately
}

bool CLI::validateNumberInput(const std::string& input, int& menuOption) const {
    try {
        if (input.empty()) {
            print("Empty input detected. Please enter a number from the menu", MessageType::ERROR);
            print("Tip: Each number corresponds to a menu option above", MessageType::INFO);
            return false;
        }
        
        std::string trimmed = sanitizeInput(input);
        if (trimmed.empty()) {
            print("Input contains only whitespace. Please enter a number from the menu", MessageType::ERROR);
            return false;
        }

        if (trimmed.length() > 10) {
            print("Input too long. Please enter a single digit", MessageType::ERROR);
            return false;
        }

        bool hasNonDigit = false;
        bool hasNegativeSign = false;
        std::string invalidChars;
        
        for (size_t i = 0; i < trimmed.length(); ++i) {
            char c = trimmed[i];
            if (c < '0' || c > '9') {
                hasNonDigit = true;
                if (c == '-' && i == 0) {
                    hasNegativeSign = true;
                } else {
                    if (invalidChars.find(c) == std::string::npos) {
                        invalidChars += c;
                    }
                }
            }
        }
        
        if (hasNonDigit) {
            if (hasNegativeSign) {
                print("Negative numbers are not allowed. Please enter a number between 0 and 4", MessageType::ERROR);
            } else if (!invalidChars.empty()) {
                print("Invalid characters detected: '" + invalidChars + "'. Please enter only digits (0-4)", MessageType::ERROR);
            } else {
                print("Invalid input format. Please enter a valid number (0-4)", MessageType::ERROR);
            }

            if (trimmed.find('.') != std::string::npos) {
                print("Tip: Decimal numbers are not allowed. Use whole numbers only", MessageType::INFO);
            }
            if (trimmed.find(' ') != std::string::npos) {
                print("Tip: Remove spaces from your input", MessageType::INFO);
            }
            
            return false;
        }

        try {
            // Use stoi with error checking
            size_t pos;
            int value = std::stoi(trimmed, &pos);

            if (pos != trimmed.length()) {
                print("Input contains invalid characters after the number", MessageType::ERROR);
                return false;
            }
            
            menuOption = value;

            if (menuOption < 0) {
                print("Negative numbers are not allowed. Please enter a number between 0 and 4", MessageType::ERROR);
                print("Available options: 0=Switch mode, 1=Help, 2=Status, 3=Clear, 4=Quit", MessageType::INFO);
                return false;
            }
            
            if (menuOption > 4) {
                print("Number too large. Please enter a number between 0 and 4", MessageType::ERROR);
                print("Available options: 0=Switch mode, 1=Help, 2=Status, 3=Clear, 4=Quit", MessageType::INFO);
                
                // Provide specific guidance for common mistakes
                if (menuOption >= 5 && menuOption <= 9) {
                    print("Tip: Only options 0-4 are available in the current menu", MessageType::INFO);
                }
                
                return false;
            }
            
            return true;
            
        } catch (const std::invalid_argument& e) {
            print("Invalid number format: " + std::string(e.what()), MessageType::ERROR);
            print("Please enter a valid integer between 0 and 4", MessageType::INFO);
            return false;
        } catch (const std::out_of_range& e) {
            print("Number is too large to process: " + std::string(e.what()), MessageType::ERROR);
            print("Please enter a number between 0 and 4", MessageType::INFO);
            return false;
        }
        
    } catch (const std::exception& e) {
        print("Unexpected error validating number input: " + std::string(e.what()), MessageType::ERROR);
        print("Please try entering a number between 0 and 4", MessageType::INFO);
        return false;
    } catch (...) {
        print("Unknown error occurred during number validation", MessageType::ERROR);
        print("Please try entering a number between 0 and 4", MessageType::INFO);
        return false;
    }
}

CLI::CommandType CLI::convertMenuOptionToCommand(int option) const {
    switch (option) {
        case 0:
            return CommandType::SWITCH_MODE;  // Switch to text mode
        case 1:
            return CommandType::HELP;         // Display help
        case 2:
            return CommandType::STATUS;       // Show status
        case 3:
            return CommandType::CLEAR;        // Clear screen
        case 4:
            return CommandType::QUIT;         // Exit CLI
        default:
            return CommandType::UNKNOWN;     // Should not happen after validation
    }
}

// Enhanced invalid numeric input handling with comprehensive error recovery
void CLI::handleInvalidNumberInput(const std::string& input) const {
    if (currentMode != Mode::NUMBER_MODE) {
        return;
    }
    try {
        print("Invalid menu selection: '" + input + "'", MessageType::ERROR);
        std::string trimmed = sanitizeInput(input);
        if (trimmed.empty()) {
            print("Empty input detected. Please select a menu option by entering its number.", MessageType::INFO);
        } else if (trimmed.length() > 1) {
            std::string lowerInput = toLowerCase(trimmed);
            bool found = false;
            for (const auto& cmd : commandRegistry) {
                if (toLowerCase(cmd.primaryName) == lowerInput) {
                    found = true;
                    break;
                }
                for (const auto& alias : cmd.aliases) {
                    if (toLowerCase(alias) == lowerInput) {
                        found = true;
                        break;
                    }
                }
                if (found) break;
            }
            if (found) {
                print("'" + trimmed + "' is a text mode command. Switch to text mode to use it.", MessageType::INFO);
                print("Tip: Enter '0' to switch to text mode", MessageType::COMMAND);
            } else {
                print("Multi-character input detected. In menu mode, enter only single digits.", MessageType::INFO);
            }
        } else {
            char c = trimmed[0];
            if (c >= 'a' && c <= 'z') {
                print("Letter '" + std::string(1, c) + "' detected. In menu mode, use numbers only.", MessageType::INFO);
                print("Tip: Enter '0' to switch to text mode for letter commands", MessageType::COMMAND);
            } else if (c >= 'A' && c <= 'Z') {
                print("Letter '" + std::string(1, c) + "' detected. In menu mode, use numbers only.", MessageType::INFO);
                print("Tip: Enter '0' to switch to text mode for letter commands", MessageType::COMMAND);
            } else if (c >= '5' && c <= '9') {
                print("Number '" + std::string(1, c) + "' is outside the valid range.", MessageType::INFO);
                print("Valid options are 0, 1, 2, 3, or 4", MessageType::COMMAND);
            } else {
                print("Special character '" + std::string(1, c) + "' is not a valid menu option.", MessageType::INFO);
            }
        }

        print("Valid menu options:", MessageType::INFO);
        print("  0 - Switch to text mode", MessageType::COMMAND);
        print("  1 - Display help", MessageType::COMMAND);
        print("  2 - Show status", MessageType::COMMAND);
        print("  3 - Clear screen", MessageType::COMMAND);
        print("  4 - Exit CLI", MessageType::COMMAND);

        print("Recovery suggestions:", MessageType::INFO);
        if (trimmed.length() > 1 || (trimmed.length() == 1 && 
            ((trimmed[0] >= 'a' && trimmed[0] <= 'z') || (trimmed[0] >= 'A' && trimmed[0] <= 'Z')))) {
            print("• Enter '0' to switch to text mode for word commands", MessageType::DESCRIPTION);
        }
        print("• Enter '1' to see detailed help information", MessageType::DESCRIPTION);
        print("• Look at the menu above and enter the corresponding number", MessageType::DESCRIPTION);
        
    } catch (const std::exception& e) {
        print("Error handling invalid input: " + std::string(e.what()), MessageType::ERROR);
        print("Please enter a number from 0 to 4", MessageType::INFO);
    } catch (...) {
        print("Unknown error handling invalid input", MessageType::ERROR);
        print("Please enter a number from 0 to 4", MessageType::INFO);
    }
}

// Color scheme management methods
void CLI::initializeColorScheme() {
    // Apply the default color scheme
    applyColorScheme(currentColorScheme);
}

void CLI::applyColorScheme(ColorScheme scheme) {
    currentColorScheme = scheme;

    if (colorsEnabled) {
        std::string schemeName;
        switch (scheme) {
            case ColorScheme::DEFAULT:
                schemeName = "Default color scheme applied";
                break;
            case ColorScheme::DARK:
                schemeName = "Dark color scheme applied";
                break;
            case ColorScheme::MATRIX:
                schemeName = "Matrix color scheme applied";
                break;
            case ColorScheme::RETRO:
                schemeName = "Retro color scheme applied";
                break;
            case ColorScheme::HIGH_CONTRAST:
                schemeName = "High contrast color scheme applied";
                break;
        }
        
        // Only show feedback if colors are enabled
        if (running) {  // Only show if CLI is running to avoid startup noise
            print(schemeName, MessageType::INFO);
        }
    }
}

// Enhanced error handling and validation methods

// Comprehensive input validation
bool CLI::validateInput(const std::string& input) const {
    // Check for null or extremely long input
    if (input.length() > 10000) {
        print("Input too long. Maximum length is 10000 characters.", MessageType::ERROR);
        return false;
    }
    
    // Check for potentially dangerous characters or sequences
    if (input.find('\0') != std::string::npos) {
        print("Input contains null characters and cannot be processed.", MessageType::ERROR);
        return false;
    }
    
    // Allow empty input (will be handled appropriately by mode-specific validation)
    return true;
}

// Validate CLI initial state before starting
bool CLI::validateInitialState() const {
    // Check if essential components are properly initialized
    if (commandRegistry.empty()) {
        print("Error: Command registry not initialized", MessageType::ERROR);
        return false;
    }
    
    // Validate current mode is set to a valid value
    if (currentMode != Mode::TEXT_MODE && currentMode != Mode::NUMBER_MODE) {
        print("Error: Invalid current mode setting", MessageType::ERROR);
        return false;
    }
    
    // Check if prompt is valid
    if (prompt.empty()) {
        print("Warning: Prompt is empty, using default", MessageType::WARNING);
        // This is not a fatal error, so we don't return false
    }
    
    return true;
}

// Validate CLI execution state before command execution
bool CLI::validateExecutionState() const {
    // Check if current mode is valid
    if (currentMode != Mode::TEXT_MODE && currentMode != Mode::NUMBER_MODE) {
        print("Error: CLI is in an invalid mode", MessageType::ERROR);
        return false;
    }
    
    // Note: We don't check 'running' flag here because commands can be processed
    // even when not in the main run loop (e.g., during testing or direct calls)
    
    return true;
}

// Validate command arguments for specific commands
bool CLI::validateCommandArguments(CommandType commandType, const std::vector<std::string>& arguments) const {
    switch (commandType) {
        case CommandType::HELP:
        case CommandType::QUIT:
        case CommandType::SWITCH_MODE:
        case CommandType::CLEAR:
        case CommandType::STATUS:
            // These commands don't require arguments, but extra arguments are ignored
            if (arguments.size() > 1) {
                print("Note: Extra arguments ignored for this command", MessageType::WARNING);
            }
            return true;
            
        case CommandType::UNKNOWN:
            print("Cannot validate arguments for unknown command", MessageType::ERROR);
            return false;
            
        default:
            // Future commands should be handled here
            print("Warning: Argument validation not implemented for this command", MessageType::WARNING);
            return true;
    }
}

// Extract command name from input for error reporting
std::string CLI::extractCommandName(const std::string& input, const std::vector<std::string>& arguments) const {
    if (!arguments.empty()) {
        return arguments[0];
    }
    
    // Fallback: extract first word from input
    std::string sanitized = sanitizeInput(input);
    size_t spacePos = sanitized.find(' ');
    if (spacePos != std::string::npos) {
        return sanitized.substr(0, spacePos);
    }
    
    return sanitized;
}

// Enhanced unknown command handling with detailed suggestions
void CLI::handleUnknownCommand(const std::string& commandName) const {
    if (commandName.empty()) {
        logger->error("Empty command entered");
        if (currentMode == Mode::TEXT_MODE) {
            print("Type 'help' to see available commands", MessageType::INFO);
        } else {
            print("Select a number from the menu above", MessageType::INFO);
        }
        return;
    }
    
    logger->error("Unknown command: '" + commandName + "'");
    
    // Provide mode-specific suggestions
    if (currentMode == Mode::TEXT_MODE) {
        // Enhanced command suggestions with multiple strategies
        std::vector<std::string> suggestions = getSimilarCommands(commandName);
        
        if (!suggestions.empty()) {
            print("Did you mean:", MessageType::INFO);
            for (const std::string& suggestion : suggestions) {
                print("  " + suggestion, MessageType::COMMAND);
            }
        } else {
            // If no similar commands, provide general help
            print("No similar commands found.", MessageType::INFO);
            
            // Check if it might be a number mode command
            if (commandName.length() == 1 && commandName[0] >= '0' && commandName[0] <= '9') {
                print("Tip: Numbers are used in menu mode. Type 'switch' to change modes.", MessageType::INFO);
            }
        }
        
        print("Type 'help' to see all available commands", MessageType::INFO);
        
    } else {
        // Number mode - different error handling
        print("In menu mode, please enter a number from 0 to 4", MessageType::INFO);
        print("Tip: Type '0' to switch to text mode for direct command entry", MessageType::INFO);
    }
}

// Emergency shutdown procedure
void CLI::handleEmergencyShutdown() {
    try {
        logger->error("Initiating emergency shutdown...");
        running = false;
        
        // Try to display exit message
        if (!exitMessage.empty()) {
            print(exitMessage, MessageType::INFO);
        } else {
            print("Emergency shutdown complete.", MessageType::INFO);
        }
        
    } catch (...) {
        // If even emergency shutdown fails, just force stop
        running = false;
        logger->error("Critical error: Emergency shutdown failed. Terminating.");
    }
}

// Provide feedback for menu selection with error checking
bool CLI::provideMenuSelectionFeedback(int menuOption) const {
    try {
        switch (menuOption) {
            case 0:
                print("Selected: Switch to text mode", MessageType::INFO);
                break;
            case 1:
                print("Selected: Display help", MessageType::INFO);
                break;
            case 2:
                print("Selected: Show status", MessageType::INFO);
                break;
            case 3:
                print("Selected: Clear screen", MessageType::INFO);
                break;
            case 4:
                print("Selected: Exit CLI", MessageType::INFO);
                break;
            default:
                print("Warning: Unknown menu option selected", MessageType::WARNING);
                return false;
        }
        return true;
    } catch (const std::exception& e) {
        print("Error providing menu feedback: " + std::string(e.what()), MessageType::ERROR);
        return false;
    }
}

// Enhanced error recovery methods

// Recover from input stream errors
void CLI::recoverFromInputError() {
    try {
        if (std::cin.fail()) {
            logger->warning("Attempting to recover from input stream error...");
            
            std::cin.clear();
            
            std::cin.ignore(1000, '\n');
            
            logger->info("Input stream recovery completed");
        }
    } catch (const std::exception& e) {
        logger->error("Error during input recovery: " + std::string(e.what()));
    } catch (...) {
        logger->error("Unknown error during input recovery");
    }
}

// Recommend action based off what the user typed - autofill or change mode
bool CLI::attemptCommandRecovery(const std::string& input) {
    try {
        print("Attempting command recovery for: '" + input + "'", MessageType::INFO);
        
        if (currentMode == Mode::TEXT_MODE) {
            std::vector<std::string> suggestions = getSimilarCommands(input);
            if (!suggestions.empty()) {
                print("Suggested recovery: Try '" + suggestions[0] + "'", MessageType::INFO);
                return true;
            }
        }
        
        if (currentMode == Mode::TEXT_MODE && input.length() == 1 &&
            input[0] >= '0' && input[0] <= '9') {
            print("Recovery suggestion: Switch to menu mode to use numbers", MessageType::INFO);
            return true;
        }
        
        if (currentMode == Mode::NUMBER_MODE && input.length() > 1) {
            print("Recovery suggestion: Switch to text mode to use word commands", MessageType::INFO);
            return true;
        }
        
        print("Recovery suggestion: Type 'help' for available commands", MessageType::INFO);
        return true;
        
    } catch (const std::exception& e) {
        print("Error during command recovery: " + std::string(e.what()), MessageType::ERROR);
        return false;
    } catch (...) {
        print("Unknown error during command recovery", MessageType::ERROR);
        return false;
    }
}

// Handle idiot operator
void CLI::analyzeCommandMistakes(const std::string& command) const {
    try {
        std::string lower = toLowerCase(command);
        
        if (lower.find("exit") != std::string::npos || lower.find("quit") != std::string::npos) {
            print("Tip: Use 'quit', 'exit', or 'q' to exit the CLI", MessageType::INFO);
        } else if (lower.find("help") != std::string::npos) {
            print("Tip: Use 'help' or 'h' to display help information", MessageType::INFO);
        } else if (lower.find("clear") != std::string::npos || lower.find("cls") != std::string::npos) {
            print("Tip: Use 'clear' or 'cls' to clear the screen", MessageType::INFO);
        } else if (lower.find("switch") != std::string::npos || lower.find("mode") != std::string::npos) {
            print("Tip: Use 'switch', 'mode', or 'm' to change between text and menu modes", MessageType::INFO);
        } else if (lower.find("status") != std::string::npos || lower.find("info") != std::string::npos) {
            print("Tip: Use 'status' or 'info' to display current CLI status", MessageType::INFO);
        }
        
        else if (lower == "ls" || lower == "dir") {
            print("This is a CLI manager, not a file system. Try 'help' for available commands.", MessageType::INFO);
        } else if (lower == "cd" || lower == "pwd") {
            print("This is a CLI manager, not a file system. Try 'help' for available commands.", MessageType::INFO);
        } else if (lower == "man" || lower.find("manual") != std::string::npos) {
            print("Use 'help' instead of 'man' to see available commands.", MessageType::INFO);
        } else if (lower == "version" || lower == "--version" || lower == "-v") {
            print("Use 'status' to see CLI information and current mode.", MessageType::INFO);
        }
        
        else if (command.length() >= 2) {
            std::vector<std::string> partialMatches;
            for (const auto& cmd : commandRegistry) {
                if (cmd.primaryName.find(lower) == 0 || lower.find(cmd.primaryName) == 0) {
                    partialMatches.push_back(cmd.primaryName);
                }
                for (const auto& alias : cmd.aliases) {
                    if (alias.find(lower) == 0 || lower.find(alias) == 0) {
                        partialMatches.push_back(alias);
                    }
                }
            }
            if (!partialMatches.empty()) {
                print("Possible partial matches found:", MessageType::INFO);
                for (const std::string& match : partialMatches) {
                    print("  " + match, MessageType::COMMAND);
                }
            }
        }
        
        if (command != lower) {
            print("Note: Commands are case-insensitive, so capitalization doesn't matter.", MessageType::INFO);
        }
        
        if (command.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") != std::string::npos) {
            print("Note: Commands should contain only letters and numbers.", MessageType::INFO);
        }
        
    } catch (const std::exception& e) {
        print("Error analyzing command mistakes: " + std::string(e.what()), MessageType::ERROR);
    } catch (...) {
        print("Unknown error during command analysis", MessageType::ERROR);
    }
}

CLI::~CLI() {
    try {
        // Cleanup if needed - currently no dynamic resources to clean up
        // But ensure we're not in a running state
        if (running) {
            running = false;
        }
    } catch (...) {
    }
}
