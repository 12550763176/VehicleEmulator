#include "DBCDatabase.h"

DBCDatabase::DBCDatabase(){

}

DBCFile* DBCDatabase::get(string key){
    return map[key];
}
bool DBCDatabase::put(string key, DBCFile* file){
    map[key] = file;
    return true;
}
bool DBCDatabase::del(string key){
    map.erase(map.find(key));
    return true;
}

void DBCDatabase::refresh_DBC(){
    //for(auto&& it: map)
}

vector<DBCFile*> DBCDatabase::getList(){
   vector<DBCFile*> vec;
   for(auto&& it : map){
       if(it.second->isParse()){
           vec.push_back(it.second);
       }
   }
   return vec;
}




bool DBCDatabase::parse(){
    if(map.size() == 0) return false;
    vector<string> tmp;
    for(auto&& it : map){
        if(it.second->isParse()){
            continue;
        }
        if(!it.second->parseDBC()){
            tmp.push_back(it.first);
        }
    }
    for(string str : tmp){
        map.erase(map.find(str));
    }

}
