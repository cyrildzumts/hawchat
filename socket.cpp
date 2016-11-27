#include "socket.h"

Socket::Socket():Socket(CLIENT)
{
}

Socket::Socket(SOCKET_USAGE_TYPE type):type{type}, stopped{false}
{
    init();
}

Socket::~Socket()
{
    close(socket_fd);
}

void Socket::init()
{
    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        std::cerr << "signal" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    port = SERVER_PORT;
    // getaddrinfo() to get a list of usable addresses
    std::string host = "localhost";
    char service[NI_MAXSERV];
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    // Work with IPV4/6
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    //hints.ai_protocol = 0;
    hints.ai_flags = (type == SERVER) ? AI_PASSIVE | AI_NUMERICSERV : AI_NUMERICSERV ;
    // we could provide a host instead of nullptr
    if(getaddrinfo((type == CLIENT) ? host.c_str() : nullptr,
                   port.c_str(),
                   &hints,
                   &result) != 0)
    {
        perror("getaddrinfo()");
        std::exit(EXIT_FAILURE);
    }
}

int Socket::createSocket()
{
    addrinfo *rp;
    int optval = 1;
    for( rp = result; rp != nullptr; rp = rp->ai_next)
    {
        socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(socket_fd == SOCKET_ERROR)
        {
            // on error we try the next address
            continue;
        }
        if(type == SERVER)
        {
            if(setsockopt(socket_fd, SOL_SOCKET,
                          SO_REUSEADDR,&optval,
                          sizeof(optval)) == SOCKET_ERROR)
            {
                perror("setsockopt");
                return SOCKET_ERROR;
            }
            if(bind(socket_fd,
                    rp->ai_addr,
                    rp->ai_addrlen) == 0)
            {
                break; // Success !
            }

        }
        else
        {
            if(connect(socket_fd,
                       rp->ai_addr,
                       rp->ai_addrlen) != SOCKET_ERROR)
            {
                //notify(new Connection{socket_fd});
                notify(&socket_fd);
                break; // success
            }
        }
        close(socket_fd);
    }
    if(rp == nullptr) // could not bind socket to any address of the list
    {
        std::cerr << "Fatal Error : couldn't find a suitable address" << std::endl;
        socket_fd = SOCKET_ERROR;
    }

    // enable socket connexions.
    // make it a socket server
    else
        if((type == SERVER) &&
                listen(socket_fd, BACKLOG) == SOCKET_ERROR)
    {
        perror("listen: ");
        socket_fd = SOCKET_ERROR;
    }
    freeaddrinfo(rp);
    return socket_fd;
}

Socket::socket_t Socket::getSocket()const
{
    return socket_fd;
}

sockaddr Socket::getSocketAddr()const
{
    return *addr;
}

void Socket::setSocketAddr(sockaddr *addr)
{
    this->addr = addr;
}


socklen_t Socket::getSocketAddrLen()const
{
    return addrlen;
}


void Socket::setSocketAddrLen(socklen_t len)
{
    addrlen = len;
}

void Socket::setPort(std::string port)
{
    this->port = port;
}

std::string Socket::getPort()const
{
    return port;
}

void Socket::stop()
{
    stopped = true;
}

void Socket::socket_accept()
{
    socklen_t addrlen;
    sockaddr_storage client_addr;
    //int client_socket_fd;
    addrlen = sizeof(sockaddr_storage);
    char addStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    int *client_socket_fd = nullptr;
    while(!stopped)
    {
        client_socket_fd = new int;
        Logger::log("waiting for new Connexion from client ");
        *client_socket_fd = accept(socket_fd, (sockaddr*)&client_addr, &addrlen);
        if(*client_socket_fd == -1)
        {
            perror("accept error");
            continue;
        }
        if(getnameinfo((sockaddr*)&client_addr,addrlen, host, NI_MAXHOST, service,
                       NI_MAXSERV, 0) == 0)
        {
            //snprintf(addStr, ADDRSTRLEN, "(%s, %s)", host, service);
            //notify(new Connection{client_socket_fd,
            //                      host, service});
            notify(client_socket_fd);
            client_socket_fd = nullptr;
        }
        else
        {
            snprintf(addStr, ADDRSTRLEN, "(?unknown)");

        }
    }
}

