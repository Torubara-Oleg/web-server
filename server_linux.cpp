#include "httpCommunication.h"
#include <thread>

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
    HTTP instance("127.0.0.1", 54124);

    //long length = 0;
    if(instance.start() == -1)
    {
        printf("FAILURE EXIT\n");
        exit(-1);
    }

    std::string com = "";
    std::thread waiter(quit,std::ref(com),std::ref(instance));

    if(instance.createEpoll() != 0)
        std::cout << "[HTTP SERVER] epoll creation failed..." << std::endl;
    
    while (true) 
    {
        instance.epollWait();
    }
   
    waiter.join();
    return 0;
}
