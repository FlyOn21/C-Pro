//
// Created by flyon21 on 09.11.24.
//

#ifndef C_PRO_LOGGER_H
#define C_PRO_LOGGER_H
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <mutex>
#include <iomanip>
#include <vector>


class Logger {
public:
    enum LogLevel {
        INFO, WARNING, ERROR
    };

    static Logger &getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string &message, LogLevel level = INFO) {
        std::ofstream logFile("log.txt", std::ios::app);
        std::vector<std::string> coloredLogLevel = getColoredLogLevelString(level);
        if (logFile.is_open()) {
            logFile << "[" << getLogLevelString(level) << "] "
                    << getCurrentTime() << " - " << message << std::endl;
            logFile.close();
        } else {
            std::vector<std::string> coloredLogLevelError = getColoredLogLevelString(ERROR);
            std::cerr << coloredLogLevelError[0] << "Error opening log file!" << coloredLogLevelError[1] << std::endl;
        }
        std::cout << coloredLogLevel[0] << "[" << getLogLevelString(level) << "] "
                  << getCurrentTime() << " - " << message << coloredLogLevel[1] << std::endl;
    }

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

private:
    Logger() = default;

    ~Logger() = default;


    static std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);
        std::tm *local_time = std::localtime(&time_now);
        std::ostringstream oss;
        oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    static std::string getLogLevelString(LogLevel level) {
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

    static std::vector<std::string> getColoredLogLevelString(LogLevel level) {
        switch (level) {
            case INFO:
                return {"\033[1;34m", "\033[0m"};
            case WARNING:
                return {"\033[1;33m", "\033[0m"};
            case ERROR:
                return {"\033[1;31m", "\033[0m"};
            default:
                return {"", ""};
        }
    }
};
#endif //C_PRO_LOGGER_H
