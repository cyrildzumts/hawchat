#include "channel.h"


Channel::Channel(int fd, int id):fd{fd}, id{id}, quit{false}
{
    //quit = false;
}

Channel::~Channel()
{
    rxd_data.clear();
    txd_data.clear();
    quit = true;
}

void Channel::stop()
{
    quit = false;
}

int Channel::getSocket()const
{
    return fd;
}



void Channel::addRxdData(std::pair<void *, int> data)
{
    rxd_data.push(data);
    notify();
}

void Channel::addTxdData(std::pair<void *, int> data)
{
    txd_data.push(data);
}


void Channel::sendData(void *data, int len)
{
    //txd_data.push(std::make_pair(data,len));
    //sendData(std::pair<void*, int>(data,len));
    writeData(data, len);
}

void Channel::sendData(const std::pair<void *, int> &data)
{
    //txd_data.push(data);
    sendData(data.first, data.second);
}



std::pair<void *, int> Channel::getRxdData()
{
    std::pair<void*, int> data;
    rxd_data.wait_and_pop(data);
    return data;
}

void Channel::operator ()()
{
    //std::thread writer{&Channel::writeData,this};
    std::thread reader{&Channel::readData,this};
    //writer.detach();
    reader.detach();

}

void Channel::writeData(void *data, int len)
{
    int count = 0;
    int writeCount = 0;
    while(count < len)
    {
        writeCount = write(fd,
                           (char*)data + count,
                           len - count);
        if(writeCount < 0)
        {
            perror("Channel writeMessage");
            exit(EXIT_FAILURE);
        }
        count += writeCount;
    }
    Logger::log("Channel: Data sent ..");
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    delete (char*)data;
    /*
         * Logger::log(std::string("message sent ")
                    + std::to_string(count)
                    + " Bytes ");
                    */
}

void Channel::writeData()
{

    int count = 0;
    int writeCount = 0;
    std::pair<void*, int> entry;
    char *data = nullptr;
    while(!quit)
    {
        txd_data.wait_and_pop(entry);
        data = (char*)entry.first;
        while(count < entry.second)
        {
            writeCount = write(fd,
                               data + count,
                               entry.second - count);
            if(writeCount < 0)
            {
                perror("Channel writeMessage");
                exit(EXIT_FAILURE);
            }
            count += writeCount;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        delete data;
        /*
         * Logger::log(std::string("message sent ")
                    + std::to_string(count)
                    + " Bytes ");
                    */
    }
}
int Channel::read_data(void **dest)
{
    int count = 0;
    char data[BUFFER_SIZE];
    memset(data, 0, BUFFER_SIZE);
    count = read(fd,data,BUFFER_SIZE );

    if(count < 0)
    {
        perror(" Error on Channel readMessage");

        quit = true;
    }

    else if(count > 0)
    {
        std::cout << __FUNCTION__ << " read " << count << " Bytes "<< std::endl;
        *dest = new char[count+1];
        memset(*dest, 0, count+1);
        memcpy(*dest,data, count);
        Logger::log(std::string((char*)(*dest)));
        memset(data, 0, count);
    }
    return count;
}

void Channel::readData()
{
    int count = 0;
    char data[BUFFER_SIZE];
    memset(data, 0, BUFFER_SIZE);
    void *data_ptr = nullptr;
    while((!quit) )
    {
        count = read(fd, data,BUFFER_SIZE );

        if(count < 0)
        {
            perror(" Error on Channel readMessage");

            quit = true;
            exit(EXIT_FAILURE);
        }

        if(count == 0)
        {
            //std::cout << __FUNCTION__ << " read " << count << " Bytes "<< std::endl;
            continue;
        }
        if(count == 1) // this a heart beat signal
        {
            // maybe the observer will like to know that the
            // client had checked to see if we are still alive
            memset(data, 0, count);
            continue;
        }
        else
        {
            std::cout << __FUNCTION__ << " read " << count << " Bytes "<< std::endl;
            data_ptr = new char[count+1];
            memset(data_ptr, 0, count+1);
            memcpy(data_ptr,data, count);
            Logger::log(std::string((char*)data_ptr));
            memset(data, 0, count);
            //addRxdData(std::pair<void*, int>(data_ptr,count));
            handler(data_ptr);

            //std::cout << __FUNCTION__ << " copying message read done !:" << std::endl;
        }
    }
}

void Channel::setHandler(const std::function<void(void*)> &handler)
{
    this->handler = handler;
}

int Channel::getID()const
{
    return id;
}
