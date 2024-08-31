#include "logger.h"

Logger& Logger::instance()
{
    static Logger inst;
    return inst;
}

void Logger::LogMessage(std::string&& message)
{
    std::cout<< message << std::endl;
}
