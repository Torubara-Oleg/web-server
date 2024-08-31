#include "http_tcpServer.h"

tcpServer::tcpServer(std::string addr, int port)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    int rc = setsockopt(m_socket, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
    if (rc < 0)
    {
        perror("[TCP-SERVER] setsockopt() failed");
        close(m_socket);
        exit(-1);
    }


    server.sin_addr.s_addr = inet_addr(addr.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

    std::cout<<"[TCP-SERVER] Server created\n";
}

tcpServer::~tcpServer()
{
    if(close(m_socket) == 0)
    {
        std::cout<<"[TCP-SERVER] socket closed"<<std::endl;
        exit(0);
    }
    else
    {
        std::cout<<"[TCP-SERVER] Error occured during socket closing " <<__FILE__<<" "<<__LINE__<<std::endl;
        exit(-2);
    }
}

int& tcpServer::getSocket()
{
    return m_socket;
}

sockaddr_in &tcpServer::getSockAddr()
{
    return server;
}

int &tcpServer::getConnection()
{
    return connfd;
}

void tcpServer::setConn(int client_socket)
{
    connfd = client_socket;
}

int tcpServer::startServer()
{
    // Binding newly created socket to given IP and verification
    if ((bind(m_socket, (sockaddr*)&server, sizeof(server))) != 0) 
    {
        std::cout<< "[TCP-SERVER] socket bind failed..."  <<__FILE__<<" "<<__LINE__ << std::endl;
        return -1;
    }
    else
        std::cout << "[TCP-SERVER] Socket successfully binded.." << std::endl;
   
    // Now server is ready to listen and verification
    if ((listen(m_socket, 10000)) != 0)
    {
        std::cout << "[TCP-SERVER] Listen failed..." <<__FILE__<<" "<<__LINE__<<std::endl;
        return -1;
    }
    else
        std::cout << "[TCP-SERVER] Server listening.." << std::endl;

    return 0;
}

int tcpServer::stopServer()
{
    close(connfd);
    close(m_socket);
    return 0;
}

int tcpServer::acceptConnection()
{
    struct sockaddr_in cli;
    int len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(m_socket, (sockaddr*)&cli, (socklen_t*)&len);
    if (connfd < 0) 
    {
        std::cout << "[TCP-SERVER] server accept failed..." <<__FILE__<<" "<<__LINE__<<std::endl;
        return -1;
    }
    #ifdef DEBUG
    else
       std::cout << "[TCP-SERVER] server accept the client..." << std::endl;
    #endif

    return connfd;
}
