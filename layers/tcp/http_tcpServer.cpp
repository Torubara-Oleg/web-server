#include "http_tcpServer.h"


#include <logger/logger.h>


constexpr std::string_view DEBUG_TAG = "[TCP-SERVER]";

tcpServer::tcpServer(std::string_view addr, uint16_t port) : m_ip_address(addr), m_port(port)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    int rc = setsockopt(m_socket, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
    if (rc < 0)
    {
        Logger::instance().LogMessage(LogLevel::ERROR, "setsockopt() failed", DEBUG_TAG);
        close(m_socket);
        exit(-1);
    }


    server.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons(m_port);

    Logger::instance().LogMessage(LogLevel::INFO, "[TCP-SERVER] Server created. Socket FD: " + std::to_string(m_socket), DEBUG_TAG);
}

tcpServer::~tcpServer()
{
    if(close(m_socket) == 0)
    {
        Logger::instance().LogMessage(LogLevel::INFO, "socket closed", DEBUG_TAG);
    }
    else
    {
        Logger::instance().LogMessage(LogLevel::ERROR, "Error occured during socket closing ", DEBUG_TAG);
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
        Logger::instance().LogMessage(LogLevel::ERROR, "socket bind failed...", DEBUG_TAG);
        return -1;
    }
    else
        Logger::instance().LogMessage(LogLevel::INFO, "Socket successfully binded..", DEBUG_TAG);
   
    // Now server is ready to listen and verification
    if ((listen(m_socket, 10000)) != 0)
    {
        Logger::instance().LogMessage(LogLevel::ERROR, "Listen failed...", DEBUG_TAG);
        return -1;
    }
    else
        Logger::instance().LogMessage(LogLevel::INFO, "Server listening at " + m_ip_address + ":" + std::to_string(m_port), DEBUG_TAG);

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
        Logger::instance().LogMessage(LogLevel::ERROR, "server accept failed...", DEBUG_TAG);
        return -1;
    }
    else
        Logger::instance().LogMessage(LogLevel::INFO, "server accept the client...", DEBUG_TAG);

    return connfd;
}
