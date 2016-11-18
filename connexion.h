#ifndef CONNEXION_H
#define CONNEXION_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/**
 * @brief The Connexion class
 * Represent a connexion between 2 peers.
 */
class Connexion
{
public:
    typedef struct sockaddr SockAddr;
    typedef struct in_addr In_Addr;
    typedef struct sockaddr_in SockAddr_In;
    typedef unsigned int port_t;
    Connexion();
    /**
     * @brief init
     * Initialize this connexion
     */

    void init();
    /**
     * @brief openConnexion buld a connexion between
     * client_sockfd and peer_sockfd.
     * @return True --> on success
     *         False --> on faillure
     */
    bool openConnexion();
    /**
     * @brief closeConnexion close this connexion
     */
    void closeConnexion();
    /**
     * @brief isAvailable
     * @return True --> client_sockfd is still connected to peer_sockfd
     */
    bool isAvailable()const;
    /**
     * @brief getSocket
     * @return the client_socket
     */
    int getSocket()const;

private:
    int INET_FAMILY;
    int client_sockfd;
    int peer_sockfd;
    port_t port;
    SockAddr addr;
    socklen_t addrlen;

    int flags;
    int backlog;


};

#endif // CONNEXION_H
