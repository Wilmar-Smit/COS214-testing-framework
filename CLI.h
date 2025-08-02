//
// CLI.h - Singleton CLI Manager Class Header
// Created for cli-manager feature
//

#ifndef CLI_H
#define CLI_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ANSI-color-codes.h"
#include "Logger.h"

// Foward declaration of logger
class Logger;

class CLI {
public:
    enum class CommandType;
    struct CommandInfo {
        CommandType type;
        std::string primaryName;
        std::vector<std::string> aliases;
        std::string description;
        std::string helpText;
        int menuPosition;  // -1 for text-only commands
    };
    
public:
    // Mode enumeration for CLI operation
    enum class Mode {
        TEXT_MODE,      // String command input mode
        NUMBER_MODE     // Numeric menu selection mode
    };
    
    // Command type enumeration for switch-case handling
    enum class CommandType {
        HELP,           // Display help information
        QUIT,           // Exit the CLI
        SWITCH_MODE,    // Switch between text/number modes
        CLEAR,          // Clear the screen
        STATUS,         // Display current system status
        UNKNOWN         // Invalid/unrecognized command
    };
    
    // Message type enumeration for color-coded output
    enum class MessageType {
        INFO,           // Cyan - general information
        SUCCESS,        // Green - successful operations
        WARNING,        // Yellow - warnings and cautions
        ERROR,          // Red - errors and failures
        PROMPT,         // Blue - user prompts
        COMMAND,        // Bright White - command text
        DESCRIPTION     // White - help descriptions
    };
    
    // Color scheme enumeration for predefined themes
    enum class ColorScheme {
        DEFAULT,        // Standard color scheme
        DARK,           // Dark theme with muted colors
        MATRIX,         // Green-based Matrix theme
        RETRO,          // Retro terminal colors
        HIGH_CONTRAST   // High contrast for accessibility
    };

private:
    // Core state management
    Mode currentMode;
    bool running;
    bool colorsEnabled;
    
    // Configuration settings
    std::string prompt;
    std::string welcomeMessage;
    std::string exitMessage;
    Mode defaultMode;
    ColorScheme currentColorScheme;
    
    // Command management
    std::vector<CommandInfo> commandRegistry;
    
    // Logger instance
    Logger* logger;
    
    // Private constructor for singleton pattern
    CLI();
    
    // Helper methods
    void initializeCommandRegistry();
    void initializeColorScheme();
    void applyColorScheme(ColorScheme scheme);
    const CommandInfo* findCommandByName(const std::string& name) const;
    const CommandInfo* findCommandByMenuPosition(int position) const;
    std::vector<CommandInfo> getMenuCommands() const;

public:
    // Singleton access method - thread-safe using static local variable
    static CLI& getInstance();
    
    // Prevent copying (singleton safety)
    CLI(const CLI&) = delete;
    CLI& operator=(const CLI&) = delete;
    
    // Core interface methods
    void run();
    bool processCommand(const std::string& input);
    void displayMenu() const;
    void displayHelp() const;
    void shutdown();
    
    // Mode management
    void switchToTextMode();
    void switchToNumberMode();
    Mode getCurrentMode() const;
    void displayCurrentMode() const;
    
    // Configuration methods
    void setPrompt(const std::string& newPrompt);
    void setWelcomeMessage(const std::string& message);
    void setExitMessage(const std::string& message);
    void enableColors(bool enable);
    void setDefaultMode(Mode mode);
    void setColorScheme(ColorScheme scheme);
    
    // Configuration validation and getters
    bool validatePrompt(const std::string& prompt) const;
    bool validateMessage(const std::string& message) const;
    Mode getDefaultMode() const;
    ColorScheme getColorScheme() const;
    const std::vector<CommandInfo>& getCommandRegistry() const;
    
    // Output methods
    void print(const std::string& message, MessageType type = MessageType::INFO) const;
    void printColored(const std::string& message, const std::string& color) const;
    void printBanner(const std::string& text) const;
    void printSeparator() const;
    
    // Input processing and command parsing methods
    CommandType parseCommand(const std::string& input);
    std::vector<std::string> parseArguments(const std::string& input);
    void executeCommand(CommandType commandType, const std::vector<std::string>& arguments);
    std::string sanitizeInput(const std::string& input) const;
    std::string toLowerCase(const std::string& str) const;
    
    // Enhanced error handling and validation methods
    bool validateInput(const std::string& input) const;
    bool validateInitialState() const;
    bool validateExecutionState() const;
    bool validateCommandArguments(CommandType commandType, const std::vector<std::string>& arguments) const;
    std::string extractCommandName(const std::string& input, const std::vector<std::string>& arguments) const;
    void handleUnknownCommand(const std::string& commandName) const;
    void handleEmergencyShutdown();
    bool provideMenuSelectionFeedback(int menuOption) const;
    
    // Enhanced error recovery methods
    void recoverFromInputError();
    bool attemptCommandRecovery(const std::string& input);
    void analyzeCommandMistakes(const std::string& command) const;
    
    // Text mode specific methods
    void displayTextModePrompt() const;
    std::vector<std::string> getSimilarCommands(const std::string& input) const;
    void suggestCommands(const std::string& invalidCommand) const;
    int calculateLevenshteinDistance(const std::string& s1, const std::string& s2) const;
    
    // Number mode specific methods
    void displayNumberModePrompt() const;
    bool validateNumberInput(const std::string& input, int& menuOption) const;
    CommandType convertMenuOptionToCommand(int option) const;
    void handleInvalidNumberInput(const std::string& input) const;
    
    // Utility methods
    void clearScreen() const;
    bool isRunning() const;
    bool supportsColors() const;
    bool detectColorSupport();
    
    // Destructor
    ~CLI();
};

#endif // CLI_H