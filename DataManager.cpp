#include "DataManager.h"

DataManager::DataManager(){

}
MsgFile* DataManager::get(string key){
    return map[key];
}
bool DataManager::put(string key, MsgFile* file){
    map[key] = file;
    return true;
}
bool DataManager::del(string key){
    map.erase(map.find(key));
    return true;
}

void DataManager::refresh_DBC(){
    //for(auto&& it: map)
}

vector<MsgFile*> DataManager::getList(){
   vector<MsgFile*> vec;
   for(auto&& it : map){
       if(it.second->isParse()){
           vec.push_back(it.second);
       }
   }
   return vec;
}




bool DataManager::parse(){
    if(map.size() == 0) return false;
    vector<string> tmp;
    for(auto&& it : map){
        if(it.second->isParse()){
            continue;
        }
        if(!it.second->parse()){
            tmp.push_back(it.first);
        }
    }
    for(string str : tmp){
        map.erase(map.find(str));
    }

}
