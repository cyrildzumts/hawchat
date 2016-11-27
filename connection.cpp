#include "connection.h"

//--------------------------------------------------------

Connection::Connection(int socket_fd): socket_fd{socket_fd}
{
    channel = nullptr;
    init();
}

Connection::Connection(int socket_fd,
                       const std::string &host,
                       const std::string &service): socket_fd{socket_fd}, host{host},service{service}
{
    channel = nullptr;
    init();
}

Connection::~Connection()
{
    std::cout << " Connection to host "
              << host << " : " << service
              << " closed " << std::endl;
    close(socket_fd);
}

void Connection::init()
{
    //channel = new Channel(socket_fd);
    //channel->addObserver(this);
}

void Connection::update()
{
    std::cout << "Channel has changed" << std::endl;
}

void Connection::update(void *args)
{
    std::cout << "Channel has changed" << std::endl;
}

void Connection::process_request()
{

}

void Connection::closeConnection()
{
    close(socket_fd);
}

bool Connection::isAvailable() const
{
    bool state = false;
    char ok = 42; // Universal response for Life !
    int errorCode;
    int count = write(socket_fd, &ok, 1);
    if (count < 0)
    {
        errorCode = errno;
        //perror(__FUNCTION__);
        if(errorCode == EPIPE)
        {
            std::cout << "Connection lost."
                      << "this connection will be removed."
                      << std::endl;
        }
    }
    else
        if(count = 1)
        {
            state = true;
        }

    return state;
}

int Connection::getSocket()const
{
    return socket_fd;
}

std::string Connection::getHost()const
{
    return host;
}
void Connection::setHost(const std::string &host)
{
    this->host = host;
}

std::string Connection::getService()const
{
    return service;
}

void Connection::setService(const std::string &service)
{
    this->service = service;
}


Channel *Connection::getChannel()
{
    return channel;
}


