#ifndef DBCDATABASE_H
#define DBCDATABASE_H
#include <unordered_map>
#include "DBCFile.h"
using namespace std;

class DataManager
{
public:
    static DataManager*  getInstace(){
        static DataManager instance;
        return &instance;
    }

    bool put(string key, MsgFile* file);
    MsgFile* get(string key);
    bool del(string key);
    bool parse();

    vector<MsgFile*> getList();

private:
    DataManager();
    DataManager(const DataManager&) = delete;
    void refresh_DBC();

    unordered_map<string, Signal*> mSigMap;
    unordered_map<uint16_t, Msg> canFrame;

    unordered_map<string, MsgFile*> map;
};

#endif // DBCDATABASE_H
