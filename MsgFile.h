#ifndef MSGFILE_H
#define MSGFILE_H
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "Utils.h"
struct ValueTable{
    string flag = "VAL_";
    uint16_t canId;
    string sigName;
    unordered_map<string, string> map;

};

//CM_ Object MessageId/NodeName “Comment”

struct Annotation{
    string flag = "CM_ SG_";
    uint16_t canID;
    string sig;
    string comment;
};
struct Signal {
    string flag = "SG_";
    uint16_t canId;
    string sigName;
    uint16_t endByte;

    uint8_t startBit;
    uint8_t sigSize;
    char byteOrder; //0代表Motorola格式，1代表Inter格式；
    char valueType; //+表示无符号数，-表示有符号数；
    string resolution;
    string Offset;
    string min;
    string max;
    string unit;
    Annotation description;
    ValueTable valueMap;
    string defaultValue = "0";
    vector<string> receiver;
    string  curValue = "";
    uint64_t Invalid;
    uint64_t moveSize;
};




enum PERMISSION{
  READ,
  WRITE,
};
enum TYPE{
  INT32_T,
  INT64_T,
  FLOAT_T,
  BYTE,
  STRING,
};
#define CAN "CAN"
#define SPI "SPI"

struct Msg {
    string flag = "BO_";
    uint16_t id;
    string name;

    unordered_map<string, Signal> sig;
    bool isSelect = false;

    int dataBytes;// data len

    string src;
    uint64_t interval = 100;  // cycle time
    string type;    //send type


    std::string channel;
    vector<uint8_t> data ;
    PERMISSION permission;



};


class MsgFile
{
public:
    //MsgFile(string path);
    virtual string getPath() = 0;
    virtual bool parse()  = 0;
    virtual bool isParse() = 0;
    virtual unordered_map<uint16_t, Msg>&  getMsg() = 0;
    virtual std::string getChannel() =  0;
    virtual ~MsgFile() = default;

};
#endif // MSGFILE_H
