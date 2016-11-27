#include "client.h"

Client::Client()
{
    quit = false;
    sock = new Socket();
    sock->addObserver(this);
    sock->init();
    user_loggedIn = false;
    usage =std::string
                (
                     " Usage : type in the message you want to send\n"
                     " and validate your action with the Enter key.\n"
                     " To log in, please enter /login + your username\n"
                     " Please notice that the username must be maximal\n"
                     " 16 character long.\n"
                     " To logout  please enter /logout and validate with \n"
                     " with Enter key."
                     " To see the list of available users to chat with, please \n"
                     " enter /users and validate with the Enter key\n"
                     " To quit this application, please enter /quit and then\n"
                     " validate with the Enter key\n"
                );

    head =  "-------------------------------------------\n"
                            "-- Welcome to Haw Chat Application "
                            "-- Copyright 2016 "
                            "-------------------------------------------\n";

}
Client::~Client()
{
    channel->stop();
    connection->closeConnection();
}

void Client::init()
{
    if(sock->createSocket() == SOCKET_ERROR)
    {
        exit(EXIT_FAILURE);
    }

   //std::thread client_read{&Client::readData, this};
   //client_read.detach();
}

void Client::login()
{
    bool username_invalid = true;
    //std::string username = "cyrildz";
    std::string username;
    Logger::log(head);
    Logger::log(usage);

    LogInOut log;

    std::this_thread::sleep_for(std::chrono::seconds(5));
    while(username_invalid && !user_loggedIn)
    {
        Logger::log( " enter a username : ");
        std::cin >> username;
        if(username.size() > STR_LEN)
        {
            Logger::log("\n username too long ");

        }
        else if(username.size() < 3)
        {
            Logger::log("\n username too short ");

        }
        else
        {
            Logger::log("username : " + username);
            log = build_loginout(username);
            void *data = Serialization::Serialize<LogInOut>::serialize(log);
            int len = log.header.length + sizeof(Header) + username.size();
            channel->sendData(data, len);
            Logger::log("Client : login sent ..");
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            len = channel->read_data(&data);
            Logger::log("Client : login received ..");
            if(len > 0)
            {
                decodeData(data);
            }
            //data = Serialization::Serialize<LogInOut>::serialize(log);
            //std::this_thread::sleep_for(std::chrono::milliseconds(800));
            //channel->sendData(data, len);
            if(user_loggedIn)
            {
                Logger::log("User logged in");
                break;
            }
        }

    }
}

void Client::logout()
{
    quit = true;
    channel->stop();
}

void Client::setUsername(const std::string &username)
{
    this->username = username;
}

std::string Client::getUsername()const
{
    return username;
}


void Client::readData()
{
    std::pair<void*, int> rxd_data ;
    while(!quit)
    {
        rxd_data = channel->getRxdData();
        decodeData(rxd_data.first);
    }
}

void Client::decodeData(void *data)
{
    Logger::log("Client decoding...");
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
        updateLogin(log);
        break;
     case MSG:
        msg = Serialization::Serialize<Message>::deserialize(data);
        std::cout << msg.sender << " : " << msg.data << std::endl;
        break;
     case CONTROLINFO:
        controlInfo = Serialization::Serialize<ControlInfo>::deserialize(data);
        break;
    }
    ptr = nullptr;
    delete[] (char*)data;

}

void Client::update()
{
    //Logger::log("message received from server: ");
    std::pair<void*, int> rxd_data = channel->getRxdData();
    decodeData(rxd_data.first);
}

void Client::update(void *args)
{

    Socket::socket_t sock =(*(Socket::socket_t*)args);
    channel = new Channel(sock);
    channel->addObserver(this);
    //(*this->channel)();
    Logger::log("Connected to server");

}

void Client::updateLogin(LogInOut &log)
{
    switch(log.header.flags)
    {
        case (SYN | ACK) :
        user_loggedIn = true;
        //username_invalid = false;
        //std::cout << "\n you are now logged in. have fun with others."
         //         << std::endl;
        Logger::log("\n you are now logged in. have fun with others.");
        break;

    case (SYN |ACK|DUP):
        Logger::log("\n username already in used.");
        break;
    case (FIN | ACK):
        Logger::log("\n you succesfully  logged out. Goodbye");
        break;

    }
}

void Client::process_message(Message &message)
{

}

void Client::process_loginout(Header &header)
{

}

void Client::process_controlInfo(ControlInfo &controlInfo)
{

}


void Client::shell()
{

    login();
    int count = 0;
    std::string line;
    Message msg;
    char *ptr = nullptr;
    int size = 0;
    while(!quit)
    {
        Logger::log(username + " : ");
        std::getline(std::cin,line);
        Logger::log("input : " + line);
        if(line == "/quit")
        {
            quit = true;
            break;
        }

        msg = build_message(username,username,line.c_str(), line.size());
        ptr = (char*)Serialization::Serialize<Message>::serialize(msg);
        size = 2 * username.size() + line.size() + sizeof(Header);
        //channel->sendData(ptr, size);
        count = write(channel->getSocket(), ptr, size);
        if(count < 0)
        {
            perror("shell write : ");
            quit = true;

        }
        if(count == size)
        {
            Logger::log("All data sent ..." + std::to_string(count) + " Bytes " );

        }
        else
        {
            Logger::log("no all data could be sent ...");
        }
        delete[] ptr;
        ptr = nullptr;
        size = 0;


    }
    Logger::log("Quitting the shell ...");

}
