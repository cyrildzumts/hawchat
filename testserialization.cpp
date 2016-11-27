#include "testserialization.h"

static std::mutex mux;
Message build_message(const std::string &sender,
                      const std::string &receiver,
                      const char *data, int len)
{
    std::lock_guard<std::mutex>lock(mux);
    //std::cout << "inside  "<< __FUNCTION__ << std::endl;
    Header header;
    header.length =  len;
    header.type = MSG;
    header.version = 0;
    header.flags = SYN | ACK;
    Message msg;
    msg.data = new char[len+1];


    memset(msg.data, 0, len+1);
    memset(msg.sender, 0, 16);
    memset(msg.receiver, 0, 16);


    memcpy(msg.receiver, receiver.c_str(), receiver.size());
    memcpy(msg.sender, sender.c_str(), sender.size());
    msg.header = header;

    memcpy(msg.data, data, len);
    //std::cout << "leaving  "<< __FUNCTION__ << std::endl;
    return msg;
}

LogInOut build_loginout(const std::string &username)
{
    LogInOut log;
    log.header.type     = LOGINOUT;
    log.header.flags    = SYN;
    log.header.version  = 0;
    log.header.length   = username.size();
    memset(log.username, 0, STR_LEN);
    strcpy(log.username, username.c_str());
    return log;
}


void print_message(const Message &msg)
{
    std::cout << __FUNCTION__ << " displaying message :" << std::endl;
    flat_header header;
    header.header = msg.header;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Message Content : " << std::endl;
    std::cout << "Message size : " << (int)msg.header.length
              << " Bytes" << std::endl;
    std::cout << "Message header : " << std::hex << header.value << std::endl;
    std::cout << "Message sender : " << msg.sender << std::endl;
    std::cout << "Message receiver : " << msg.receiver << std::endl;
    std::cout << "Message : " << msg.data << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << std::dec;
}

void messageTest(const void *data)
{
    std::cout << __FUNCTION__ << std::endl;
     Header header;
     /*
     header.length = (Byte)data[0];
     header.version = data[1];
     header.flags = data[2];
     header.type = data[3];
*/
     int len = 0;
     char sender[STR_LEN];
     char receiver[STR_LEN];

     memcpy(&header, data, 4);
     len = header.length;
     char buffer[len+1];
     memset(buffer, 0, len+1);
     memcpy(sender, data+4, STR_LEN);
     memcpy(receiver, data+ 20, STR_LEN);
     memcpy(buffer, data+36, len);
     Message msg = build_message(std::string(sender),std::string(receiver),buffer, len );
     std::cout << __FUNCTION__ << " printing message "<< std::endl;
     int buflen = strlen(buffer);
     print_message(msg);

}

void printLogInout(const LogInOut &log)
{
    std::cout << __FUNCTION__ << " displaying LogInOut :" << std::endl;
    flat_header header;
    header.header = log.header;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "LogInOut Content : " << std::endl;
    std::cout << " size : " << (int)log.header.length
              << " Bytes" << std::endl;
    std::cout << "LogInOut header : " << std::hex << header.value << std::endl;
    std::cout << "Username : " << log.username << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << std::dec;
}

void loginout_test()
{
    using namespace Serialization;
    std::cout << __FUNCTION__ << std::endl;
    Serialize<LogInOut>serializer;
    std::string username = "Cyrildz";
    LogInOut login = build_loginout(username);
    printLogInout(login);
    void *data = serializer.serialize(login);
    std::cout << "Serialization Test " << std::endl;
    LogInOut login_ser = serializer.deserialize(data);
    printLogInout(login_ser);
}

void message_test()
{
    using namespace Serialization;
    Serialize<Message> msg_serializer;
    std::cout << __FUNCTION__ << std::endl;
    std::string buffer = "Hello Server";
    std::string receiver = "Cyrildz";
    std::string sender = "Cyrille";
    Message msg = build_message(sender, receiver,buffer.c_str(), buffer.size());
    print_message(msg);
    std::cout << "Serialization Test " << std::endl;
    void *data = msg_serializer.serialize(msg);
    Message deser_msg = msg_serializer.deserialize(data);
    print_message(deser_msg);
}

