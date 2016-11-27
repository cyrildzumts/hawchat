#ifndef SOCKET_H
#define SOCKET_H

#include "common.h"
#include "connection.h"

class Socket : public Subject
{
public:
    enum SOCKET_USAGE_TYPE
    {
        SERVER,
        CLIENT
    };

    typedef int socket_t;
    Socket();
    explicit Socket(SOCKET_USAGE_TYPE type);
    ~Socket();
    void init();
    socket_t getSocket()const;
    sockaddr getSocketAddr()const;
    void setSocketAddr(sockaddr *addr);
    socklen_t getSocketAddrLen()const;
    void setSocketAddrLen(socklen_t len);
    void setPort(std::string port);
    std::string getPort()const;
    void socket_close();
    void stop();
    int createSocket();

    // Server Interface
    int socket_bind();
    int socket_listen();
    void socket_accept();

    // Client Interface
    int socket_connect();

    // Communication Interface
    // send data to anyone connected to this socket
    void sock_send(char *data, size_t len);
    // receive data from anyone connected to this socket
    char *sock_recv();


private:
    bool stopped;
    SOCKET_USAGE_TYPE type;
    socket_t socket_fd;
    sockaddr *addr;
    socklen_t addrlen;
    addrinfo hints;
    addrinfo *result;
    int flags;
    std::string port;

};

#endif // SOCKET_H
