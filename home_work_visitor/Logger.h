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
#include <mutex>


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
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream logFile("log.txt", std::ios::app);
        ColorCode coloredLogLevel = getColorCode(level);

        if (logFile.is_open()) {
            logFile << "[" << getLogLevelString(level) << "] "
                    << getCurrentTime() << " - " << message << std::endl;
            logFile.close();
        } else {
            ColorCode coloredLogLevelError = getColorCode(ERROR);
            std::cerr << coloredLogLevelError.start << "Error opening log file!" << coloredLogLevelError.end << std::endl;
        }
        std::cout << coloredLogLevel.start << "[" << getLogLevelString(level) << "] "
                  << getCurrentTime() << " - " << message << coloredLogLevel.end << std::endl;
    }

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

private:
    Logger() = default;

    ~Logger() = default;

    std::mutex mutex_;

    static std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);
        std::tm local_time_buf{};
        std::tm* local_time = localtime_r(&time_now, &local_time_buf);
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

    struct ColorCode {
        const char* start;
        const char* end;
    };

    static ColorCode getColorCode(LogLevel level) {
        switch (level) {
            case INFO:    return {"\033[1;34m", "\033[0m"};
            case WARNING: return {"\033[1;33m", "\033[0m"};
            case ERROR:   return {"\033[1;31m", "\033[0m"};
            default:      return {"", ""};
        }
    }
};
#endif //C_PRO_LOGGER_H
