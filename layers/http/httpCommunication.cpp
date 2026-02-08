#include "httpCommunication.h"
#include <cstring>
#include <logger/logger.h>

constexpr std::string_view DEBUG_TAG = "[HTTP]";

std::string HTTP::readImageFile(std::string image_path) 
{
    std::ifstream file("." + image_path, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening image file." << __FILE__<<" "<<__LINE__<<std::endl;
        return "";
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

void HTTP::setClient(int client_socket)
{
    tcp->setConn(client_socket);
}

std::string HTTP::textFileParse(std::string fileName)
{
    std::string packet;
    std::ifstream file;

    file.open("." + fileName);

    packet.append("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
    if (file) 
    {
        // get length of file:
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);

        char* buffer = new char [length];
        #ifdef DEBUG
        std::cout << "Reading " << length << " characters... "<<std::endl;
        #endif
        // read data as a block:
        file.read (buffer,length);

        if (file)
        {
            #ifdef DEBUG
            std::cout << "all characters read successfully."<<std::endl;
            #endif
        }
        else
            std::cout << "error: only " << file.gcount() << " could be read " <<__FILE__<<" "<<__LINE__<<std::endl;

        packet.append(std::to_string(length)+"\n\n");
        packet.append(buffer);
        file.close();
    }
    return packet;
}


void HTTP::sendTextToBrowser(std::string fileName)
{
    m_httpPacket = textFileParse(fileName);
    //send...
    write(tcp->getConnection(), m_httpPacket.c_str(), strlen(m_httpPacket.c_str()));
}

void HTTP::sendImageToBrowser(std::string path) 
{
    std::string image_data = readImageFile(path);
    if (image_data.empty()) {
        return;
    }

    std::string http_response = "HTTP/1.1 200 OK\r\n";
    switch(type)
    {
        case ResourceType::JPEG:
        {
            http_response += "Content-Type: image/jpeg\r\n";
            break;
        }
        case ResourceType::PNG:
        {
            http_response += "Content-Type: image/png\r\n";
            break;
        }
        default:
            break;
    }
    http_response += "Content-Length: " + std::to_string(image_data.length()) + "\r\n\r\n";

    // Convert the response string to a C-style char array to use with send()
    const char* response_cstr = http_response.c_str();
    int response_length = http_response.length();
    send(tcp->getConnection(), response_cstr, response_length, 0);

    // Convert the image data string to a C-style char array to use with send()
    const char* image_data_cstr = image_data.c_str();
    int image_data_length = image_data.length();
    send(tcp->getConnection(), image_data_cstr, image_data_length, 0);
}

HTTP::HTTP(std::string_view ipAddr, uint16_t port)
{
    tcp = std::make_unique<tcpServer>(ipAddr, port);
    events.resize(MAX_EVENTS);
    std::cout<<"[HTTP SERVER] turning on"<<std::endl;
}

HTTP::~HTTP()
{
    std::cout<<"[HTTP SERVER] shuting down"<<std::endl;
}

int HTTP::start()
{
    if(tcp->startServer() == -1)
        return -1;
    return 0;
}

int &HTTP::getConnection() const
{
    return tcp->getConnection();
}

int &HTTP::getSocket() const
{
    return tcp->getSocket();
}

int HTTP::recieveRequest()
{
    long read_value = read(tcp->getConnection(), buffer, 1024);
    printf("\n[HTTP request]----------------BEGIN---------------\n\n");
    printf("%s\n",buffer);
    printf("[HTTP request]----------------END-----------------\n\n");
    return 0;
}

int HTTP::acceptConnection()
{
    if(tcp->acceptConnection() == -1)
        std::cout<<"[HTTP SERVER] Accept error"<<__FILE__<<" "<<__LINE__<<std::endl;
    else
        return tcp->getConnection();
}

void HTTP::requestProcessing(std::string fileName)
{
#ifdef DEBUG
    std::cout<<"[HTTP SERVER] Resourse path: "<<fileName<<std::endl;
#endif
    setType(getFileExtension(fileName));

    switch(type)
    {
        case ResourceType::HTML:
        {
            sendTextToBrowser(fileName);
            break;
        }
        case ResourceType::JPEG:
        {
            sendImageToBrowser(fileName);
            break;
        }
        case ResourceType::PNG:
        {
            sendImageToBrowser(fileName);
            break;
        }
        default:
            break;
    }
}

int HTTP::handleClient()
{
    recieveRequest();

    requestProcessing(getResoursePath(std::string(buffer)));

    close(tcp->getConnection()); 
}

std::string HTTP::getResoursePath(std::string httpRequest)
{
    std::string resoursePath;
    bool flag = false;

    for(auto ch : httpRequest)
    {
        if(ch == ' '&& flag == true)
            break;

        if(ch == '/'&& flag ==false)
            flag = true;
        
        
        if(flag)
        {
            resoursePath+=ch;
        }
    }
    return resoursePath;
}

int HTTP::createEpoll()
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("[HTTP SERVER] epoll_create1 error");
        exit(EXIT_FAILURE);
    }

    std::memset(&event, 0, sizeof(event)); // Zero out the struct
    event.data.fd = getSocket();
    event.events = EPOLLIN; // Interested in read events
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, getSocket(), &event) == -1)
    {
         perror("[HTTP SERVER] epoll_ctl ADD listening socket error");
         exit(EXIT_FAILURE);
    }
    Logger::instance().LogMessage(LogLevel::INFO, "Added listening socket " + std::to_string(getSocket()) + " to epoll", DEBUG_TAG);
    return 0;
}

int HTTP::epollWait()
{
    int num_events = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
    if (num_events == -1) 
    {
        if (errno == EINTR)
            return 0;
        perror("[HTTP SERVER] epoll_wait error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_events; ++i) 
    {
        Logger::instance().LogMessage(LogLevel::INFO, "Event [" + std::to_string(i) + "] FD=" + std::to_string(events[i].data.fd) + " Events=" + std::to_string(events[i].events), DEBUG_TAG);
        
        if (events[i].data.fd == getSocket()) 
        {
            // New incoming connection
            int client_socket = acceptConnection();
            Logger::instance().LogMessage(LogLevel::INFO, "New client connected:" + std::to_string(client_socket), DEBUG_TAG);
            
            struct epoll_event event;
            std::memset(&event, 0, sizeof(event));
            event.data.fd = client_socket;
            event.events = EPOLLIN;
            Logger::instance().LogMessage(LogLevel::INFO, "Adding client " + std::to_string(client_socket) + " to epoll", DEBUG_TAG);
            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1) {
                Logger::instance().LogMessage(LogLevel::ERROR, "Failed to add client to epoll: " + std::to_string(errno), DEBUG_TAG);
            }
            handleClient();
        } 
        else
        {
            // Handle data from connected clients
            int client_socket = events[i].data.fd;
            Logger::instance().LogMessage(LogLevel::INFO, "Handling event for client: " + std::to_string(client_socket), DEBUG_TAG);
            
            setClient(client_socket);
            
            //handleClient();

            // Note: handleClient closes the socket, so we don't need to modify epoll event 
            // as closing automatically removes it (if it's the last reference).
            // epoll_ctl(epoll_fd, EPOLL_CTL_MOD, client_socket, &event);
        }
    }
}

void HTTP::setType(std::string extension)
{
    if(extension == "html")
        type = ResourceType::HTML;
    if(extension == "jpg" || extension == "jpeg")
        type = ResourceType::JPEG;
    if(extension == "png")
        type = ResourceType::PNG;
}
