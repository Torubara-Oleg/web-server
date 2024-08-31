#ifndef TCP_SERVER
#define TCP_SERVER

#include <iostream>
#include "netUtils.h"
#include <string.h>

class tcpServer
{
public:
    tcpServer(std::string addr, int port);
    ~tcpServer();

    int& getSocket();
    sockaddr_in& getSockAddr();
    int& getConnection(); 


    void setConn(int client_socket);
    int startServer();
    int stopServer();
    int acceptConnection();
private:

    int m_socket, connfd;
    int port;
    std::string ip_addres;
    struct sockaddr_in server;
    int on = 1; //to make socket reusable
};
#endif 
