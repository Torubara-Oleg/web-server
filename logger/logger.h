#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <source_location>

enum class LogLevel
{
    ERROR,
    WARNING,
    INFO,
    // DEBUG
};

class Logger
{    
public:
    static Logger& instance();
    ~Logger(){}
    void LogMessage(const LogLevel level, std::string&& message, const std::string_view& tag,
                            const std::source_location& location = std::source_location::current());
private:
    Logger() = default;
    Logger(Logger&) = delete;


};

#endif