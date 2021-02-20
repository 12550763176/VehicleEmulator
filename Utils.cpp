#include "Utils.h"

Utils::Utils()
{

}

vector<string> Utils::split(string str, string pattern1, string pattern2){
    vector<string> vec;
    uint32_t len = str.size();
    for(uint32_t i = 0; i < len; i++){
        uint32_t pos1 = str.find(pattern1, i);
        uint32_t pos2 = str.find(pattern2, i);
        uint32_t pos = pos1 > pos2 ?  pos2 : pos1;
        if(pos > 0 && pos < len){
            string s = str.substr(i, pos-i );
            i=pos;
            if(s.empty()) continue;
            vec.push_back(s);
        } else{
            if(str.substr(i, len-i).empty()) continue;
            vec.push_back(str.substr(i, len-i));
            return vec;
        }
    }
    return vec;
}

vector<string> Utils::split(string str, string pattern){
    vector<string> vec;
    uint32_t len = str.size();
    for(uint32_t i = 0; i < len; i++){
        uint32_t pos = str.find(pattern, i);
        if(pos > 0 && pos < len){
            string s = str.substr(i, pos-i );
            i=pos;
            vec.push_back(s);
        } else{
            vec.push_back(str.substr(i, len-i));
            return vec;
        }
    }
    return vec;
}

string Utils::trim(string &s)
 {

     if( !s.empty() )
     {
        s.erase(0,s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
     if( !s.empty() )
     {
        s.erase(0,s.find_first_not_of("\t"));
        s.erase(s.find_last_not_of("\t") + 1);
    }
     if( !s.empty() )
     {
        s.erase(0,s.find_first_not_of("\n\r"));
        s.erase(s.find_last_not_of("\n\r") + 1);
    }
     if( !s.empty() )
     {
        s.erase(0,s.find_first_not_of("\n"));
        s.erase(s.find_last_not_of("\n") + 1);
    }
    return s;
}

 bool Utils::isNum(string str)
 {
     for (int i = 0; i < str.size(); i++)
        {
            int tmp = (int)str[i];
            if ((tmp >= 48 && tmp <= 57) || tmp == '-')
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        return true;
}

string Utils::trim(string str, string del)
{
    if( !str.empty() )
    {
       str.erase(str.find_last_not_of(del.c_str()) + 1);
   }
}

