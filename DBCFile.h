#ifndef DBCFILE_H
#define DBCFILE_H
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "MsgFile.h"
#include "Utils.h"


using namespace std;


//struct ValueTable{
//    string flag = "VAL_";
//    uint16_t canId;
//    string sigName;
//    unordered_map<string, string> map;

//};

////CM_ Object MessageId/NodeName “Comment”

//struct Annotation{
//    string flag = "CM_ SG_";
//    uint16_t canID;
//    string sig;
//    string comment;
//};

////SG_ VCU_PT_WakeupReasons : 31|8@0+ (1,0) [0|255] "" Vector__XXX
////SG_ SignalName : StartBit|SignalSize@ByteOrder ValueType (Factor,Offset) [Min|Max] Unit Receiver

//struct Signal {
//    string flag = "SG_";
//    uint16_t canId;
//    string sigName;
//    uint16_t endByte;

//    uint8_t startBit;
//    uint8_t sigSize;
//    char byteOrder; //0代表Motorola格式，1代表Inter格式；
//    char valueType; //+表示无符号数，-表示有符号数；
//    string resolution;
//    string Offset;
//    string min;
//    string max;
//    string unit;
//    Annotation description;
//    ValueTable valueMap;
//    string defaultValue = "0";
//    vector<string> receiver;
//    string  curValue = "";
//    uint64_t Invalid;
//    uint64_t moveSize;
//};


//struct CanFrame {
//    string flag = "BO_";
//    uint16_t canId;
//    string canName;
//    uint8_t dataBytes;
//    string src;
//    uint64_t interval = 100;
//    string type;
//    unordered_map<string, Signal> sig;
//    vector<uint8_t> data ;
//    bool isSelect = false;
//};



//BA_DEF_ Object AttributeName ValueType Min Max;
//BA_DEF_ SG_  "GenSigInactiveValue" INT 0 0;

struct Attribute {
    string flag = "BA_DEF_ SG_";
    string target; //object
    string name; //AttributeName
    string ValueType;  //ValueType
    vector<string> value;
};

// BA_DEF_DEF_ AttributeName DefaultValue;
// BA_DEF_DEF_  "NmMessageCount" 128;
struct DefaultVaule{
    string flag = "BA_DEF_DEF_";
    string target; //object
    string defalut;
};

struct DBC {
    string  VERSION;
    string owner;
    string secretkey;

    vector<string> NS; //new symbol
    string BS; //bps baudrate  ...
    //vector<string> BU;
    string BU;
    unordered_map<uint16_t, Msg> canFrame;
};


class DBCFile  :  public MsgFile
{
public:
    DBCFile(string path);
    string getPath() override;
    bool parse() override;
    bool isParse() override;
    unordered_map<uint16_t, Msg>& getMsg() override;
    std::string getChannel() override;

private:
    DBC mDbc;
    string mPath;
    bool parsed = false;
    unordered_map<string, Signal*> mSigMap;
    string mIDs;

    bool checkKey(string key);

    void parseOWNER(string tmp);
    void parseKEY(string tmp);
    void parseNS(ifstream& in);
    void parseBS(ifstream& in);
    void parseBU(ifstream& in, string tmp);
    void parseBO(ifstream& in, string tmp);
    void parseCM(ifstream& in, string tmp);
    void parseVT(ifstream& in, string tmp);
    void parseVD(ifstream& in, string tmp);
    void parseInterval(ifstream& in, string tmp);

};

#endif // DBCFILE_H
