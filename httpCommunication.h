#include "http_tcpServer.h"
#include "fsUtils.h"
#include <memory>

#define MAX_EVENTS 10000

enum class ResourceType
{
    HTML = 0,
    JPEG,
    PNG
};

class HTTP
{
public:
    HTTP(std::string ipAddr, int port);
    ~HTTP();

    int start();

    int createEpoll();
    
    int epollWait();
private:
    int handleClient();

    void setClient(int client_socket);

    int& getConnection() const;

    int& getSocket() const;

    int recieveRequest();

    int acceptConnection();

    void sendImageToBrowser(std::string path);

    std::string textFileParse(std::string fileName);

    void sendTextToBrowser(std::string fileName);

    void requestProcessing(std::string fileName);

    std::string getResoursePath(std::string httpRequest);

    std::string readImageFile(std::string image_path) ;

    void setType(std::string extension);

private:
    std::unique_ptr<tcpServer> tcp;
    char buffer[1024]={0};   //contains http request data
    std::string m_httpPacket;
    ResourceType type;

    //epoll
    int epoll_fd;
    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];
};