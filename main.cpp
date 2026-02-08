#include "httpCommunication.h"
#include "logger/logger.h"

#include "utils/utils.h"

#include <thread>

constexpr std::string_view DEBUG_TAG = "[MAIN]";

volatile sig_atomic_t server_running = 1;

void signal_handler(int sig_num)
{
    server_running = 0;
}

void quit(std::string& command, HTTP& inst)
{
    while(command != "quit")
    {
        std::cin >> command;
    }

    inst.~HTTP();
    exit(EXIT_SUCCESS);
}

int main()
{
    constexpr std::string_view ip = "127.0.0.1";
    constexpr uint16_t port = 54124;


    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    HTTP instance(ip, port);
    Logger& log = Logger::instance();
    if(instance.start() == -1)
    {
        log.LogMessage(LogLevel::ERROR, "FAILURE EXIT", DEBUG_TAG);
        return -1;
    }

    std::string com = "";
    std::thread waiter(quit,std::ref(com),std::ref(instance));
    waiter.detach();

    if(instance.createEpoll() != 0)
        log.LogMessage(LogLevel::ERROR, "[HTTP SERVER] epoll creation failed...", DEBUG_TAG);
    
    while (server_running) 
    {
        instance.epollWait();
    }
   
    log.LogMessage(LogLevel::INFO, "Server shutting down...", DEBUG_TAG);
    return 0;
}
