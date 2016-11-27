#ifndef CLIENT_H
#define CLIENT_H

#include "logger.h"
#include "socket.h"



class Client : public Observer
{
public:
    Client();
    ~Client();
    void init();
    void login();
    void setUsername(const std::string &username);
    std::string getUsername()const;
    void logout();
    void sendMessage(const Message &message);
    void readMessage();
    void readData();
    void decodeData(void *data);

    Message getMessage();
    void shell();

    // Observer interface
public:
    virtual void update() override;
    virtual void update(void *args) override;

private:
    void updateLogin(LogInOut &log);
    void process_message(Message &message);
    void process_loginout(Header &header);
    void process_controlInfo(ControlInfo &controlInfo);

private:
    bool user_loggedIn;
    bool quit;
    bool username_invalid;
    Connection *connection;
    Channel *channel;
    Socket *sock;
    std::string username;
    std::string usage;
    std::string head;



};

#endif // CLIENT_H
