#ifndef DATAMANAG_H
#define DATAMANAG_H
#include "DataManager.h"
#include "DBCFile.h"
#include<stdlib.h>
#include <thread>
#include <bits/std_mutex.h>
#include <random>

using std::default_random_engine;
using std::uniform_int_distribution;

class DataCalcManager
{

public:
    bool updateData(Msg& msg, string sigName, string sigValue);
    void send(Msg& msg);
    void del(Msg& msg);
    void randomSend();
    void addRandom(Msg& msg, string sigName);
    void delRandom(Msg& msg, string sigName);

    void calcValue(Msg& frame);
    static DataCalcManager& getInstance() {
        static DataCalcManager instance;
        return instance;
    }
private:
    DataCalcManager();
    ~DataCalcManager();
    vector<MsgFile *> vec;
    thread mRandom;
    unordered_map<string, Msg*> randomMap;
    std::mutex mapMutex;  // protects g_i

};

#endif // DATAMANAG_H
