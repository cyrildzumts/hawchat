#include "server.h"

Server::Server()
{
    sock = new Socket(Socket::SERVER);
    sock->addObserver(this);
    quit = false;
    userlist.clear();

}


Server::~Server()
{

    stop();
    delete sock;
    quit = true;
}

void Server::init()
{
    int thread_count = std::thread::hardware_concurrency();
    Logger::log("Maximal concurrent Thread on this machine : " + std::to_string(thread_count));
    for (int i = 0; i < thread_count; i++)
    {

    }
    int fd = sock->createSocket();
    if(fd == SOCKET_ERROR)
    {
        std::cerr << " a server socket couldn't be created."
                  << std::endl;
        exit(EXIT_FAILURE);
    }

}
void Server::start()
{
    sock->socket_accept();
}

void Server::stop()
{
    sock->stop();
}

void Server::service_1()
{

}

void Server::heartbeat()
{

}

void Server::addChannel(Channel &channel)
{



}

void Server::removeChannel(Channel &channel)
{

}

void Server::readData()
{

}
/*
void Server::decodeData(void *data, Channel *channel)
{
    Logger::log("Decoding new data received");
    char *ptr = (char*)data;
    LogInOut log;
    Message msg;
    ControlInfo controlInfo;
    flat_header header;
    memcpy(&header.value, ptr, sizeof(Header));
    switch(header.header.type)
    {
    case LOGINOUT:
        log = Serialization::Serialize<LogInOut>::deserialize(data);
        updateUser(log);
        break;
    case MSG:
        msg = Serialization::Serialize<Message>::deserialize(data);
        std::cout << msg.sender << " : " << msg.data << std::endl;
        break;
    case CONTROLINFO:
        controlInfo = Serialization::Serialize<ControlInfo>::deserialize(data);
        break;
    }

    delete[] ptr;
    ptr = nullptr;
}
*/

std::pair<void *, int> Server::decodeData(void *data, Channel *const channel)
{
    std::pair<void *, int> ret;
    Logger::log("Decoding new data received");
    char *ptr = (char*)data;
    LogInOut log;
    Message msg;
    ControlInfo controlInfo;
    flat_header header;
    memcpy(&header.value, ptr, sizeof(Header));
    switch(header.header.type)
    {
    case LOGINOUT:
        log = Serialization::Serialize<LogInOut>::deserialize(data);
         ret = updateUser(log, channel);
        break;
    case MSG:
        msg = Serialization::Serialize<Message>::deserialize(data);
        std::cout << msg.sender << " : " << msg.data << std::endl;
        break;
    case CONTROLINFO:
        controlInfo = Serialization::Serialize<ControlInfo>::deserialize(data);
        break;
    }

    delete[] ptr;
    ptr = nullptr;
    return ret;
}

void Server::update()
{
    Logger::log("new data received");
    //std::pair<void*, int> rxd_data = channel->getRxdData();
    //decodeData(rxd_data.first);
}

void Server::update(void *args)
{
    Socket::socket_t sock =(*(Socket::socket_t*)args);
    std::thread worker {&Server::request_handler, this, sock};
    worker.detach();

}

void Server::request_handler(int sock_fd)
{
    bool quit = false;
    std::pair<void*, int> reply;
    Logger::log("new handler started");
    int count = 0;
    static int channelID = 0;
    using std::placeholders::_1;
    User user;
    char *buffer = nullptr;
    Channel channel = Channel(sock_fd, channelID++);
    user.setChannel(&channel);
    user.setUsername("UnknownUser"
                     + std::to_string(channel.getID()));

    add_user(&user);
    user.getChannel()->addObserver(this);
    while(!quit)
    {
        count = channel.read_data((void**)(&buffer));
        if(count < 0)
            quit = true;
        else
        {
            if (count > 0)
            {
                reply = decodeData(buffer, &channel);
                Logger::log("Server replying to client ...");
                channel.sendData(reply);
            }
        }
    }
}

std::pair<void *, int> Server::updateUser(LogInOut &log, Channel *channel)
{
    std::pair<void *, int> ret;
    switch (log.header.flags) {
    case (SYN):
        ret = addUser(log.username, channel);
        break;
    case FIN:
        ret = removeUser(log.username);
    default:
        break;
    }
    return ret;
}

User* Server::getUser(Channel *channel)
{
    User *result = nullptr;
    for(User* user : connected_users)
    {
        if(user->getChannel()->getID() == channel->getID())
        {
            result = user;
            Logger::log("User found through channel : Name : " + user->getUsername());
            break;
        }
    }
    return result;
}

User* Server::getUser(const std::string &username)
{
    User *result = nullptr;
    for(User* user : connected_users)
    {
        if(user->getUsername() == username)
        {
            result = user;
            Logger::log("User found through username");
            break;
        }
    }
    return result;
}

void Server::add_user(User *user)
{
    std::lock_guard<std::mutex>lock(user_shield);
    connected_users.push_back(user);
    //users[user->getUsername()] = user->getChannel();
}


std::pair<void *, int> Server::addUser(const std::string &username, Channel *channel)
{
    std::pair<void *, int> ret;
    ret.second = 4;
    LogInOut log;
    log.header.length = 0;
    log.header.type = LOGINOUT;
    log.header.version = 0;
    memset(log.username, 0, STR_LEN);

    // first check to see if we have an user
    // associated to this channel
    User *user = getUser(channel);
    User *user2 = getUser(username);
    if(user)
    {
        if(user2)
        {
            log.header.flags = SYN | ACK | DUP;
            Logger::log("user " + username + " already in use.");
        }
        else
        {
            user->setUsername(username);
            userlist[username] = channel->getSocket();
            users[username] = channel;
            log.header.flags = SYN | ACK ;
            Logger::log("user " + username + " added.");
        }

    }
    ret.first = Serialization::Serialize<LogInOut>::serialize(log);
    return ret;
}



std::pair<void *, int> Server::removeUser(const std::string &username)
{
    std::pair<void *, int> ret;
    ret.second = 4;
    LogInOut log;
    log.header.length = 0;
    log.header.type = LOGINOUT;
    log.header.version = 0;
    memset(log.username, 0, STR_LEN);
    for(auto it = userlist.begin(); it != userlist.end();)
    {
        if(it->first == username)
        {
            it = userlist.erase(it);
            log.header.flags = FIN | ACK ;
            ret.first = Serialization::Serialize<LogInOut>::serialize(log);

            Logger::log("user " + username + " removed.");
        }
        else
            it++;
    }

    return ret;
}

void Server::sendMessage(const Message &msg)
{

}

std::pair<void *, int> Server::updateInfo(const ControlInfo &info)
{

}
