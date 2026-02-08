#include "logger.h"

Logger& Logger::instance()
{
    static Logger inst;
    return inst;
}

void Logger::LogMessage(const LogLevel level, std::string&& message, const std::string_view& tag, const std::source_location& location)
{
    switch (level)
    {
        case LogLevel::ERROR:
            std::cout << "[ERROR] " << tag << " " << message << " at " 
                      << location.file_name() << " " << location.line() << " " 
                      << location.function_name() << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] " << tag << " " << message << " at " 
                      << location.file_name() << " " << location.line() << " " 
                      << location.function_name() << std::endl;
            break;
        case LogLevel::INFO:
            std::cout << "[INFO] " << tag << " " << message << std::endl;
            break;
        // case LogLevel::DEBUG:
        //     std::cout << "[DEBUG] " << tag << " " << message << std::endl;
        //     break;
    }
}
