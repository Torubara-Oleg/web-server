#include "httpCommunication.h"
#include "logger/logger.h"

#include "utils/utils.h"

#include <thread>

/* first, here is the signal handler */
void catch_int(int sig_num)
{
    /* re-set the signal handler again to catch_int, for next time */
    signal(SIGINT, catch_int);
    /* and print the message */
    printf("Don't do that");
    fflush(stdout);
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

    signal(SIGINT, catch_int);

    HTTP instance("127.0.0.1", 54125);
    Logger& log = Logger::instance();
    if(instance.start() == -1)
    {
        log.LogMessage("FAILURE EXIT");
        exit(-1);
    }

    std::string com = "";
    std::thread waiter(quit,std::ref(com),std::ref(instance));

    if(instance.createEpoll() != 0)
        log.LogMessage("[HTTP SERVER] epoll creation failed...");
    
    while (true) 
    {
        instance.epollWait();
    }
   
    waiter.join();
    return 0;
}
