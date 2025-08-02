//
// Logger.cpp - Singleton Logger Class Implementation
// Created by Michael Tomlinson
//

#include "Logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance(const std::string& filename) {
    static Logger instance(filename); // Static for thread-safe singleton initialization
    return &instance;
}

Logger::Logger(const std::string& filename) : logFileName(filename) {
    logFile.open(logFileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Warning: Could not open log file '" << logFileName 
                  << "'. Logging to console only." << std::endl;
    }
}

std::string Logger::getCurrentTimestamp() {
    // Get current time - uses iostream not CHRONO
    time_t rawTime;
    time(&rawTime);
    
    struct tm* timeInfo = localtime(&rawTime);
    
    char buffer[32]; // Sufficient buffer size for [YYYY-MM-DD HH:MM:SS] + null terminator
    strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeInfo);
    
    return std::string(buffer);
}

std::string Logger::getLogLevelString(LogLevel level) {
    switch (level) {
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

std::string Logger::getLogLevelColor(LogLevel level) {
    switch (level) {
        case INFO:
            return WHT;
        case WARNING:
            return YEL;
        case ERROR:
            return RED;
        default:
            return WHT;
    }
}

void Logger::writeToConsole(const std::string& message, LogLevel level) {
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = getLogLevelString(level);
    std::string colorCode = getLogLevelColor(level);

    std::cout << colorCode << timestamp << " [" << levelStr << "] " << message << CRESET << std::endl;
}

void Logger::writeToFile(const std::string& message) {
    if (!logFile.is_open()) {
        return;
    }
    
    try {
        std::string timestamp = getCurrentTimestamp();
        
        logFile << message << std::endl;
        
        logFile.flush(); // Ensure data is written to disk immediately
        
        if (logFile.fail()) {
            // Log write failed, report to console and continue
            std::cerr << "Error: Failed to write to log file '" << logFileName 
                      << "'. Continuing with console logging only." << std::endl;
            
            // Close the file to prevent further write attempts
            logFile.close();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred while writing to log file: " << e.what()
                  << ". Continuing with console logging only." << std::endl;
        
        if (logFile.is_open()) {
            logFile.close();
        }
    }
}

void Logger::log(const std::string& message, LogLevel level) {
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = getLogLevelString(level);
    
    writeToConsole(message, level);
    
    std::string fileMessage = timestamp + " [" + levelStr + "] " + message;
    writeToFile(fileMessage);
}

void Logger::info(const std::string& message) {
    log(message, INFO);
}

void Logger::warning(const std::string& message) {
    log(message, WARNING);
}

void Logger::error(const std::string& message) {
    log(message, ERROR);
}

void Logger::clearLogs() {
    try {
        if (logFile.is_open()) {
            logFile.close();
        }
        
        logFile.open(logFileName, std::ios::out | std::ios::trunc);
        
        if (!logFile.is_open()) {
            std::cerr << "Error: Failed to clear log file '" << logFileName
                      << "'. Could not open file for truncation." << std::endl;
            return;
        }
        
        logFile.close();
        logFile.open(logFileName, std::ios::app);
        
        if (!logFile.is_open()) {
            std::cerr << "Error: Failed to reopen log file '" << logFileName
                      << "' after clearing. Continuing with console logging only." << std::endl;
            return;
        }
        
        info("Log file cleared successfully");
        
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred while clearing log file: " << e.what()
                  << ". Continuing with existing log file." << std::endl;
        
        if (!logFile.is_open()) {
            logFile.open(logFileName, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "Warning: Could not reopen log file '" << logFileName 
                          << "'. Logging to console only." << std::endl;
            }
        }
    }
}

void Logger::printRaw(const std::string& message) {
    std::cout << message << std::endl;
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}