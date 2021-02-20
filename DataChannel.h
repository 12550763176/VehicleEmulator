#ifndef DATACHANNEL_H
#define DATACHANNEL_H
#include "ConQueue.h"
#include "SocketMoudle.h"
#include <thread>
#include <mutex>
#include <vector>
#include <unordered_map>
#include "MsgFile.h"

using namespace std;

struct Frame{
    uint16_t id;
    vector<uint8_t> data;
};


class DataChannel{
public:
    uint32_t timeInterval;
    DataChannel(uint32_t interval, string type);
    uint16_t get(uint16_t  id);
    void put(uint16_t id, vector<uint8_t>& data);
    void del(uint16_t id);

    uint32_t getInterval();


private:
    mutable mutex mLock;

    vector<uint8_t> mVec;
    vector<uint8_t> tmpVec;
    unordered_map<uint16_t , vector<uint8_t>>  mCanMap;
    ConQueue<Frame> mQueue;
    thread mThreadRx;
    thread mThreadTx;

    string channelType;

    SocketMoudle* mSocket;

    void composeData();
    void txMsg();
    void rxMsg();

};




#endif // DATACHANNEL_H
