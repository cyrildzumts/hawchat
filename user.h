#ifndef USER_H
#define USER_H
#include "channel.h"

class User
{
public:
    User();
    ~User();
    /**
     * @brief setUsername set the username
     * @param username
     */
    void setUsername(const std::string &username);
    /**
     * @brief getUsername
     * @return the username of this user
     */
    std::string getUsername()const;
    /**
     * @brief setChannel
     * @param channel the channel associated to this user
     */
    void setChannel(Channel *channel);
    /**
     * @brief getChannel
     * @return the associated channel
     */
    Channel *getChannel()const;
    /**
     * @brief getIP
     * @return this user IP address
     */
    std::string getIP()const;
    /**
     * @brief setIP changed to the IP address of this user
     * @param ip
     */
    void setIP(const std::string &ip);
    /**
     * @brief setPort changed the port to which this user
     * communicates
     * @param port
     */
    void setPort(const std::string &port);
    /**
     * @brief getPort
     * @return the communication port of this user
     */
    std::string getPort()const;
    /**
     * @brief getSocketFD
     * @return the socked descriptor of this user
     */
    int getSocketFD()const;
    /**
     * @brief isConnected
     * @return the connexion status of this user.
     * True --> The user is currently connected7
     * False --> The user is not connected
     */
    bool isConnected()const;


private:
bool connected;
std::string username;
Channel *channel;
std::string ip;
std::string port;

};

#endif // USER_H
