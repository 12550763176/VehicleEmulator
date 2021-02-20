#ifndef MDBFILE_H
#define MDBFILE_H

#include "MsgFile.h"



class MDBFile : public  MsgFile
{
public:
    MDBFile(string path);
    string getPath() override;
    bool parse()  override;
    bool isParse() override;
    std::string getChannel() override;
    unordered_map<uint16_t, Msg>& getMsg() override;
private:
    string mPath;
    bool parsed = false;
    unordered_map<string, Signal*> mSigMap;
    unordered_map<uint16_t, Msg> canFrame;

};

#endif // MDBFILE_H
