#ifndef TCP_SERVER
#define TCP_SERVER

#include <iostream>
#include "netUtils.h"
#include <string.h>

class tcpServer
{
public:
    tcpServer(std::string_view addr, uint16_t port);
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
    int m_port;
    std::string m_ip_address;
    struct sockaddr_in server;
    int on = 1; //to make socket reusable
};
#endif 
