#include "Channel.h"


void Channel::send(uint64_t interval, uint16_t id, vector<uint8_t>& data){
//    printf(" %d   id:%x  data:%x \n",   interval, id, data);
    if(id == 0) return;
    if(mChannelMap.find(interval) ==  mChannelMap.end()){
        createChannel(interval)->put(id, data);
    }else{
        mChannelMap[interval]->put(id, data);
    }
}

void Channel::del(uint64_t interval, uint16_t id){
   //printf("%d  %d   id:%x  data:%zu \n", isSend,  interval, id, data);
    if(id == 0) return;

    if(mChannelMap.find(interval) ==  mChannelMap.end()){
        return;
    }else{
        mChannelMap[interval]->del(id);
    }
}


DataChannel* Channel::createChannel(uint64_t interval){
    if(mChannelMap.find(interval) ==  mChannelMap.end()){
        mChannelMap[interval] = new DataChannel(interval, getType());
    }
    return mChannelMap[interval];
}


