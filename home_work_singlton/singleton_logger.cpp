//
// Created by flyon21 on 31.10.24.
//
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
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new Logger();
        }
        return *instance;
    }

    void log(const std::string &message, LogLevel level = INFO) {
        std::lock_guard<std::mutex> lock(logMutex);
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

    static Logger *instance;
    static std::mutex mtx;
    std::mutex logMutex;
};

Logger *Logger::instance = nullptr;
std::mutex Logger::mtx;

class ConfigManager {
public:
    static ConfigManager &getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new ConfigManager();
        }
        return *instance;
    }

    void setConfig(const std::string &key, const std::string &value) {
        std::lock_guard<std::mutex> lock(configMutex);
        configData_[key] = value;
    }

    std::string getConfig(const std::string &key) const {
        std::lock_guard<std::mutex> lock(configMutex);
        auto it = configData_.find(key);
        if (it != configData_.end()) {
            return it->second;
        } else {
            return "Key not found";
        }
    }

    ConfigManager(const ConfigManager &) = delete;

    ConfigManager &operator=(const ConfigManager &) = delete;

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    static ConfigManager *instance;
    static std::mutex mtx;
    mutable std::mutex configMutex;
    std::map<std::string, std::string> configData_;
};

ConfigManager *ConfigManager::instance = nullptr;
std::mutex ConfigManager::mtx;

int main() {
    Logger::getInstance().log("This is an info message", Logger::INFO);
    Logger::getInstance().log("This is a warning message", Logger::WARNING);
    Logger::getInstance().log("This is an error message", Logger::ERROR);

    ConfigManager::getInstance().setConfig("database_host", "localhost");
    ConfigManager::getInstance().setConfig("database_port", "5432");

    std::cout << "Database Host: " << ConfigManager::getInstance().getConfig("database_host") << std::endl;
    std::cout << "Database Port: " << ConfigManager::getInstance().getConfig("database_port") << std::endl;
    std::cout << "API Key: " << ConfigManager::getInstance().getConfig("api_key") << std::endl;

    return 0;
}
