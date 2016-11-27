#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "message.h"

namespace Serialization
{
    template <typename T>
    class Serialize
    {
    public:
       static void *serialize(const T &param);
       static T deserialize(void *data);
    };

    // Specialization
    template<>
    class Serialize<Message>
    {
    public:
        static void* serialize(const Message &message)
        {
            int data_size = strlen(message.data) ;
            int size = 2*STR_LEN + sizeof(Header) + data_size ;
            char *data;
            data = new char[size+1];
            memset(data, 0, size );
            memcpy(data, (char*)&message, size - data_size);
            memcpy(data + size - data_size,(char*) message.data, data_size);
            return data;
        }

       static  Message deserialize(void *data)
        {
            Message message;
            int size = 2*STR_LEN + sizeof(Header);
            memcpy(&message,data, size );
            message.data = new char[message.header.length +1];
            memset(message.data, 0, message.header.length+1);
            memcpy(message.data, (char*)data + size, message.header.length);
            return message;
        }
    };


    template<>
    class Serialize<LogInOut>
    {
    public:
       static void* serialize(const LogInOut &log)
        {
            int size = STR_LEN + sizeof(Header);
            char *data;
            int len = strlen(log.username);
            data = new char[size];
            memset(data, 0, STR_LEN);
            memcpy(data, &log, len + sizeof(Header));

            return data;
        }

        static LogInOut deserialize(void *data)
        {

            LogInOut log;
            char *ptr = (char*)data;
            int size = ptr[0] + sizeof(Header);
            memset(log.username, 0, STR_LEN);
            memcpy(&log,data, size );
            return log;
        }
    };

    template<>
    class Serialize<ControlInfo>
    {
    public:
       static void* serialize(const ControlInfo &controlInfo)
        {
            return nullptr;
        }

       static ControlInfo deserialize(void *data)
        {

        }
    };


    template<>
    class Serialize<Entry>
    {
    public:
       static void* serialize(const Entry &entry)
        {
            return nullptr;
        }

       static Entry deserialize(void *data)
        {

        }
    };
}
#endif // SERIALIZATION_H
