#include "user.h"

User::User()
{
    channel = nullptr;
    connected = false;
    username = "";
    port = "";
    ip = "0.0.0.0";
}

User::~User()
{
    channel = nullptr;
}

void User::setChannel(Channel *channel)
{
    this->channel = channel;
    int sk = channel->getSocket();
    int flag = 1;
    setsockopt(sk, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
}

Channel* User::getChannel()const
{
    return channel;
}

std::string User::getIP()const
{
    return ip;
}

void User::setIP(const std::string &ip)
{
    this->ip = ip;
}

std::string User::getUsername()const
{
    return username;
}

void User::setUsername(const std::string &username)
{
    this->username = username;
}

void User::setPort(const std::string &port)
{
    this->port = port;
}


std::string User::getPort()const
{
    return port;
}


int User::getSocketFD()const
{
    return (channel != nullptr) ? channel->getSocket(): -1;
}
