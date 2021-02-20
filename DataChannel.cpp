#include "DataChannel.h"
#include <unistd.h>

DataChannel::DataChannel(uint32_t interval, string type){
    timeInterval = interval;
    mSocket = &(SocketMoudle::getInstance());
    channelType = type;
    mThreadRx = thread(&DataChannel::rxMsg, this);

    mThreadTx = thread(&DataChannel::txMsg, this);
}

void DataChannel::txMsg(){
    while(true) {
        if(mSocket->isConnect(channelType) && mVec.size() > 0){
            std::unique_lock<std::mutex> g(mLock);
            mSocket->write(channelType, mVec);
        }
        //DEBUG
        usleep(10 *  1000);
        //usleep(timeInterval *  1000);

    }
}

uint32_t DataChannel::getInterval(){
    return timeInterval;
}



uint16_t DataChannel::get(uint16_t  id){
    return id; //TODO
}
void DataChannel::del(uint16_t id){
    if(id == 0) return;
    if(mCanMap.find(id) ==  mCanMap.end()){
        return;
    }
    mCanMap.erase(mCanMap.find(id));
    composeData();
}



void DataChannel::put(uint16_t id, vector<uint8_t>& data){
    if(id == 0) return;

    Frame frame = {id, data};
    //printf("%x, %zu\n", frame.id, frame.data);
    mQueue.push(frame);
}




void DataChannel::composeData(){
    tmpVec.clear();
    for(std::pair<uint16_t, vector<uint8_t>> it : mCanMap){//  for (i=umap.begin();i!=umap.end();i++
        tmpVec.push_back((it.first >> 8));
        tmpVec.push_back((it.first &0xff));


//        tmpVec.push_back((it.second >> 56)&0xff);
//        tmpVec.push_back((it.second >> 48)&0xff);
//        tmpVec.push_back((it.second >> 40)&0xff);
//        tmpVec.push_back((it.second >> 32)&0xff);
//        tmpVec.push_back((it.second >> 24)&0xff);
//        tmpVec.push_back((it.second >> 16)&0xff);
//        tmpVec.push_back((it.second >> 8)&0xff);
//        tmpVec.push_back((it.second >> 0)&0xff);

        for(uint8_t v : it.second){
            tmpVec.push_back(v);
        }
/*
        printf("DataChannel::composeData \n\n");
        for(uint8_t i : tmpVec){
            printf(" %02x ", i);
        }
        printf(" \n ");
*/
    }


    {
        std::unique_lock<std::mutex> g(mLock);
        mVec = tmpVec;
    }
}

void DataChannel::rxMsg(){
    while(true){
        mQueue.waitForItems();
        std::vector<Frame> items = mQueue.flush();
        for(Frame frame : items){
            mCanMap[frame.id] = frame.data;
        }
        composeData();
    }
}



