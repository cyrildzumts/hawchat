#ifndef CONNEXION_H
#define CONNEXION_H

#define _BSD_SOURCE

#include "common.h"
#include "channel.h"
#include "observer.h"

#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
/**
 * @brief The Connection class
 * Represent a connection between 2 peers.
 */

extern int errno;

class Connection : public Observer
{
public:
    explicit Connection(int socket_fd);
    explicit Connection(int socket_fd, const std::string &host,
                        const std::string &service);

    ~Connection();
    /**
     * @brief init
     * Initialize this connection
     */

    void init();

    // Observer interface
    virtual void update() override;
    virtual void update(void *args) override;

    /**
     * @brief openConnection buld a connection between
     * client_sockfd and peer_sockfd.
     * @return True --> on success
     *         False --> on faillure
     */
    //bool openConnection();
    /**
     * @brief closeConnection close this connection
     */
    void closeConnection();
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

    Channel *getChannel();
    void setHost(const std::string &host);
    std::string getHost()const;
    void setService(const std::string &service);
    std::string getService()const;

    void process_request();

private:

private:
    Channel *channel;
    int socket_fd;
    std::string host;
    std::string service;


};

#endif // CONNEXION_H
