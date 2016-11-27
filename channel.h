#ifndef CHANNEL_H
#define CHANNEL_H

#include "logger.h"
#include "queue.h"
#include "message.h"
#include "testserialization.h"
#include "serialization.h"
#include "subject.h"

#define BUFFER_SIZE 256

class Channel : public Subject
{
public:
    Channel(int fd, int id=1);
    ~Channel();
    int getSocket()const;
    void addRxdData(std::pair<void*, int> data);
    void addTxdData(std::pair<void*, int> data);
    std::pair<void*, int> getRxdData();

    void sendData(void* data, int len);
    void sendData(const std::pair<void*, int> &data);
    void operator()();
    void stop();
    void heartbeat();
    void writeData();
    void writeData(void* data, int len);
    void readData();
    int read_data(void **dest);
    void setId(int id);
    int getID()const;
    void setHandler(const std::function<void(void*)> &handler);
private:
    void setSocket(int sock);

private:
    int id;
    bool quit;
    std::function<void(void*)> handler;
    Utils::Queue<std::pair<void*, int>> rxd_data;
    Utils::Queue<std::pair<void*, int>> txd_data;
    int fd;
};

#endif // CHANNEL_H
