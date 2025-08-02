//
// Logger.h - Singleton Logger Class Header
// Created by Michael Tomlinson
//

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "ANSI-color-codes.h"

class Logger {
    
public:
    enum LogLevel {
        INFO,
        WARNING,
        ERROR
    };

private:
    static Logger* instance;
    std::ofstream logFile;
    // Custom ticking because std::chrono is not allowed
    std::map<std::string, clock_t> activeTicks;
    std::string logFileName;
    Logger(const std::string& filename = "application.log");
    
    std::string getCurrentTimestamp();
    std::string getLogLevelString(LogLevel level);
    std::string getLogLevelColor(LogLevel level);
    void writeToConsole(const std::string& message, LogLevel level);
    void writeToFile(const std::string& message);

public:
    static Logger* getInstance(const std::string& filename = "application.log");
    
    // Prevent copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    void log(const std::string& message, LogLevel level = INFO);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void printRaw(const std::string& message);

    void clearLogs();
    
    ~Logger();
};

#endif // LOGGER_H