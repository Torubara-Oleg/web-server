#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>

class Logger
{    
public:
    static Logger& instance();
    ~Logger(){}
    void LogMessage(std::string&& message);
private:
    Logger() = default;
    Logger(Logger&) = delete;
};

#endif