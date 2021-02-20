#ifndef CHANNEL_H
#define CHANNEL_H
#include "DataChannel.h"
#include "MsgFile.h"


class Channel
{
public:
    static Channel*  getInstace(string type){
        if(type == SPI){
            static Channel instance_spi(type);
            return &instance_spi;
        } else if(type == CAN){
            static Channel instance_can(type);
            return &instance_can;
        }
    }
    void send(uint64_t interval, uint16_t id, vector<uint8_t>& data);
    void del(uint64_t interval, uint16_t id);
    string getType(){ return channelType;}



private:
    Channel(string type):channelType(type){};

    string channelType;

    DataChannel* createChannel(uint64_t interval);
    unordered_map<uint64_t, DataChannel*> mChannelMap;
};

#endif // CHANNEL_H
