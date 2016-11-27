#ifndef SERVER_H
#define SERVER_H
#include "socket.h"
#include "user.h"

/**
 * @brief The Server class
 * This class handle the services with want to offer
 * This class allow you to process client request.
 *      *This class allow you to log in /out users.
 *      *transmit message to a logged in user
 *      *update and  read the list of available
 *      *user
 *
 */

class Server : Observer
{
public:
    Server();
    ~Server();
    /**
     * @brief init initialize this server
     */
    void init();
    /**
     * @brief start start this server
     */
    void start();
    void stop();
    void service_1();
    /**
     * @brief heartbeat send an heartbeat
     * signal to every client connected to this
     * server to detect wo is still connected
     */
    void heartbeat();
    /**
     * @brief addChannel add a new Channel
     * every connected client is associated
     * to a channel through which they communicate
     * @param channel a communication channel
     * with a given user
     */
    void addChannel(Channel &channel);
    /**
     * @brief removeChannel delete a communication
     * channel. this happen when a connexion with a client
     * is lost
     * @param channel the communication channel to be
     *  deleted
     */
    void removeChannel(Channel &channel);
    /**
     * @brief readData get any received data
     * and process it
     */
    void readData();
    /**
     * @brief decodeData decode the received data and
     * call the appropriate service handler
     * @param data the received data from a client
     */
    std::pair<void *, int>decodeData(void *data);
    std::pair<void *, int> decodeData(void *data, Channel *const channel);
    void request_handler(int sock_fd);

    // Observer interface
    virtual void update() override;
    virtual void update(void *args) override;

private:
    /**
     * @brief getUser search for the user associated
     * with channel
     * @param channel the channel associated to
     * the searched user
     * @return a pointer the user when found.
     *         nullptr when no user could be found
     */
    User *getUser(Channel *channel);
    User *getUser(const std::string &username);
    std::pair<void *, int> updateUser(LogInOut &log, Channel *channel);
    /**
     * @brief addUser add a new user to this server
     * @param username the user's username to be
     * added
     * TO-DO : This method need to know which channel
     * to use to send the result of the resquested action
     * something like :
     * addUser(username, channel) where channel
     * represents the client who has mad this request
     */
    std::pair<void *, int> addUser(const std::string &username, Channel *channel);
    void add_user(User *user);
    /**
     * @brief removeUser remove the user associated with
     * username
     * @param username
     */
    std::pair<void *, int> removeUser(const std::string &username);
    /**
     * @brief sendMessage send msg to the receiver
     * @param msg message to be sent
     */
    void sendMessage(const Message &msg);
    /**
     * @brief updateInfo update the userlist
     * using info as reference
     * @param info contains a list of user
     * and how to reach them
     */
    std::pair<void*, int> updateInfo(const ControlInfo &info);

private:
    bool quit ;
    std::mutex user_shield;
    std::vector<User*>connected_users;
    std::vector<Connection*>conns;
    Connection* connection;
    Channel *channel;
    Socket *sock;
    std::map<std::string, int> userlist;
    std::map<std::string,Channel*> users;
    std::vector<std::thread> workers;

};

#endif // SERVER_H
