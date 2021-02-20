#include "DBCFile.h"

#include <QCryptographicHash>
#include <QTextCodec>
#include <qmessagebox.h>

DBCFile::DBCFile(string path) {
    mPath = path;
}

string DBCFile::getPath(){
    return mPath;
}



bool  DBCFile::parse(){
    ifstream input( mPath, ios::in );
    if (!input){
        return 1;
    }
    string tmp;
    while(getline(input, tmp)){
        //if(tmp.empty()) break;
        //printf("line: %s\n", tmp.c_str());

        if(strncmp(tmp.c_str(), "VERSION", strlen("VERSION"))==0){
            vector<string> vec = Utils::split(tmp.c_str(), " ");
            mDbc.VERSION  = vec[1];
        } else if(strncmp(tmp.c_str(), "NS_", strlen("NS_")) == 0){
            parseNS(input);
        } else if(strncmp(tmp.c_str(), "BS_", strlen("BS_")) == 0){
            //parseBS(input);
            vector<string> vec = Utils::split(tmp, ":");
            if(vec.size() > 1){
                mDbc.BS = vec[1];
            }
        } else if(strncmp(tmp.c_str(), "BU_", strlen("BU_ ")) == 0){
           parseBU(input, tmp);
        } else if(strncmp(tmp.c_str(), "BO_ ", strlen("BO_ ")) == 0){
            parseBO(input, tmp);
        } else if(strncmp(tmp.c_str(), "CM_ SG_", strlen("CM_ SG_"))  == 0){
            parseCM(input, tmp);
        } else if(strncmp(tmp.c_str(), "VAL_", strlen("VAL_")) == 0){
            parseVT(input, tmp);
        } else if(strncmp(tmp.c_str(), "BA_ \"GenSigStartValue\" SG_", strlen("BA_ \"GenSigStartValue\" SG_")) == 0){
            parseVD(input, tmp);
        }else if(strncmp(tmp.c_str(), "BA_ \"GenMsgCycleTime\" BO_", strlen("BA_ \"GenMsgCycleTime\" BO_")) == 0) {
            parseInterval(input, tmp);
        } else if(strncmp(tmp.c_str(), "OWNER :", strlen("OWNER :")) ==0 ){
            parseOWNER(tmp);
        } else if(strncmp(tmp.c_str(), "KEY :", strlen("KEY :")) ==0 ){
            parseKEY(tmp);
        }

    }
//    if(!checkKey(mIDs)){
//        mDbc.canFrame.clear();
//        return false;
//    };

    parsed = true;
    return true;
}

bool DBCFile::checkKey(string str) {
    QByteArray in, out;
    in = str.c_str();
    out = QCryptographicHash::hash(in, QCryptographicHash::Md5);
    //out.toHex().toStdString().c_str()

    //printf("key:(%s), md5:(%s)\n", mDbc.secretkey.c_str(), out.toHex().toStdString().c_str());

    if(strcmp(mDbc.secretkey.c_str(), out.toHex().toStdString().c_str())){
        QMessageBox::about(NULL, "About", "Unrecognized DBC");
        return false;
    }
    return true;

}

void DBCFile::parseOWNER(string tmp){
    mDbc.owner = Utils::split(tmp, ":")[1];
}
void DBCFile::parseKEY(string tmp){
    mDbc.secretkey = Utils::split(tmp, ":")[1];
    mDbc.secretkey = Utils::trim(mDbc.secretkey);
    //printf("%s\n",  mDbc.secretkey.c_str()0- );
}


void DBCFile::parseInterval(ifstream& in, string tmp){
    vector<string> vec = Utils::split(tmp, "BO_");
    vec = Utils::split(vec[1], " ");

    mDbc.canFrame[atoi(vec[1].c_str())].interval = atoi(vec[2].substr(0, vec[2].length() - 1).c_str());

    //printf("%s %d \n",vec[1].c_str(), mDbc.canFrame[atoi(vec[1].c_str())].interval);
}


void DBCFile::parseNS(ifstream& in){
    string tmp;
    while(getline(in, tmp)){
        if(strncmp(tmp.c_str(), " ", strlen(" ")) != 0){
            return;
        }
        mDbc.NS.push_back(tmp);
    }
}

void DBCFile::parseBS(ifstream& in){
    string tmp;
    while(getline(in, tmp)){
        if(strncmp(tmp.c_str(), " ", strlen(" ")) != 0){
            return;
        }
        //printf("%s\n", tmp.c_str());
        mDbc.BS = tmp;
    }
}

void DBCFile::parseBU(ifstream &in, string tmp){
   // string tmp;
    //while(getline(in, tmp)){
    //    if(tmp.empty())  return;
        //mDbc.BU = Utils::split(tmp, " ");
        mDbc.BU = Utils::split(tmp, ":")[1];
        //printf("%s\n", mDbc.BU.c_str());
    //}
}



void DBCFile::parseBO(ifstream &in, string line){
//    Msg* frame  = (struct Msg*)malloc(sizeof(struct Msg));
//    memset(frame, 0, sizeof(struct Msg));
//    frame->sig = unordered_map<string, Signal*>();
    Msg frame;
    vector<string> vec = Utils::split(line, " ", ":");
    frame.id = atoi(vec[1].c_str());
    frame.name = vec[2].c_str();
    frame.channel = CAN;
    frame.dataBytes = atoi(vec[3].c_str());
    frame.src = vec[4].c_str();
    //printf("parseBO %d %s %d \n", frame->id, frame->name.c_str(), frame->dataBytes);
    mIDs+=vec[1];

    while(getline(in, line)){
        //printf("|%s, id: %d %d \n", line.c_str(), line.find_last_not_of(" "), line.length());
        line.erase(0, line.find_first_not_of(" "));
        //line.erase(line.find_last_not_of(" "));
        if(strncmp(line.c_str(), "SG_", strlen("SG_")) != 0)  break;

        line.erase(0, line.find_first_not_of(" "));
        line.erase(line.find_last_not_of(" "));
        if(strncmp(line.c_str(), "SG_", strlen("SG_")) != 0)  break;

        Signal sig;

        vector<string> vec = Utils::split(line, " ", ":");
        sig.sigName = vec[1];

        sig.canId = frame.id;
        vector<string> tmp =  Utils::split(vec[2].c_str(), "|");
        sig.startBit = atoi(tmp[0].c_str());

        tmp = Utils::split(tmp[1].c_str(), "@");
        sig.sigSize = atoi(tmp[0].c_str());
        //printf("%s, sigsize:%d\n ", line.c_str(), sig.sigSize );
        sig.byteOrder = tmp[1].c_str()[0];
        sig.valueType = tmp[1].c_str()[1];

        tmp = Utils::split(vec[3].c_str(), ",");
        sig.resolution = tmp[0].substr(1).c_str();
        sig.Offset = tmp[1].substr(0, strlen(tmp[1].c_str())-1).c_str();

        tmp = Utils::split(vec[4].c_str(), "|");
        sig.min = tmp[0].substr(1);
        sig.max = tmp[1].substr(0, strlen(tmp[1].c_str())-1);

        sig.unit = vec[5].c_str();

        sig.receiver = Utils::split(vec[6].c_str(), ",");

        uint64_t tmpValue = 1;


        //sig.moveSize = 64 - sig.sigSize-(((sig.startBit>>3)<<3) + (8 - (sig.startBit&7))) +1;
        //sig.moveSize = 64 - sig.sigSize-(((sig.startBit/8)*8) + (8 - (sig.startBit%8))) +1;
        //sig.moveSize = 64 - sig.sigSize-((sig.startBit&(~7)) + (8 - (sig.startBit&7))) +1;
        //sig.moveSize = 64 +1 -8 - sig.sigSize-(sig.startBit&(~7)) + (sig.startBit&7);
        //sig.moveSize = 57 - sig.sigSize-(sig.startBit&(~7)) + (sig.startBit&7); -------m01
        //sig.moveSize = (64 - ((sig.startBit>>3)<<3) ) - (8 - sig.startBit%8  + sig.sigSize) + 1;

        //uint64_t key2 = (key1  << (56 -(frame.sig[sigName].startBit/8) *8  + frame.sig[sigName].startBit%8)  -frame.sig[sigName].sigSize);

        if(sig.sigSize <= ((sig.startBit%8)+1)) {
            sig.moveSize = sig.startBit%8 - sig.sigSize + 1;
            sig.endByte = sig.startBit/8;
        } else {
            sig.moveSize = 8 - ((sig.sigSize - (sig.startBit%8 +1))%8 == 0? 8: (sig.sigSize - (sig.startBit%8 +1))%8);
            sig.endByte = sig.startBit/8 + (sig.sigSize-(sig.startBit%8+1))/8 + ((((sig.sigSize-(sig.startBit%8 + 1))%8) == 0) ?  0: 1);

        }

        for(int i  = 0  ; i <(sig.sigSize -1); i++){
            tmpValue <<= 1;
            tmpValue |= 1;
        }
        sig.Invalid = 0xffffffffffffffff - (tmpValue << sig.moveSize);
        frame.sig[sig.sigName] = sig;
        mSigMap[sig.sigName] = &sig;
    }
    mIDs += frame.sig.size();

    mDbc.canFrame[frame.id] = frame;


}

void DBCFile::parseCM(ifstream &in, string line){

    string tmp;
    Annotation ann;

    while(line.find_last_of(";") == -1){
        getline(in, tmp);
        line += "\n";
        line += tmp;

    }

    vector<string> vec = Utils::split(line, " ");
    ann.canID = atoi(vec[2].c_str());
    ann.sig = vec[3].c_str();


    for(int i = 4; i < vec.size(); i++){
        ann.comment += vec[i].c_str();
        ann.comment += " ";
    }
    //printf("%s\n", ann.comment.c_str());
    mDbc.canFrame[ann.canID].sig[ann.sig].description = ann;
}

void DBCFile::parseVT(ifstream &in, string line){
    string tmp;

    while(line.find_last_of(";") == -1){
        getline(in, tmp);
        line += tmp;
        line += " ";
    }

    //printf("%s\n", line.c_str());

    vector<string> vec = Utils::split(line, " ");
    ValueTable  vt;
    vt.canId = atoi(vec[1].c_str());
    vt.sigName = vec[2];
    tmp.clear();
    for(int i = 3; i < vec.size(); i++){
        tmp += vec[i];
        tmp += " ";
    }
    vec = Utils::split(tmp, "\" ");
    for(int i = 0; i < vec.size(); i++){
        vector<string> value = Utils::split(vec[i].c_str(), " \"");
        if(value.size() < 2) continue;
        vt.map[value[0]] = (value[1] += "\"");
    }
    mDbc.canFrame[vt.canId].sig[vt.sigName].valueMap = vt;
    //printf("%d\n", mDbc.canFrame[vt.canId].sig[vt.sigName].valueMap.map.size());
}

void DBCFile::parseVD(ifstream &in, string tmp){
    vector<string> vec = Utils::split(tmp, " ");
    mDbc.canFrame[atoi(vec[3].c_str())].sig[vec[4]].defaultValue = Utils::split(vec[5], ";")[0];

}

unordered_map<uint16_t, Msg>& DBCFile::getMsg(){
    //return unordered_map<uint16_t, Msg*>();
   return mDbc.canFrame;
}

bool DBCFile::isParse(){
    return  parsed;
}

std::string DBCFile::getChannel(){
    return  CAN;
}
