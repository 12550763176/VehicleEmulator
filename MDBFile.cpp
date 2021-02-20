#include "MDBFile.h"
#include <libxl.h>

MDBFile::MDBFile(string path)
{

    this->mPath = path;
}
string MDBFile::getPath(){
    return this->mPath;
}
bool MDBFile::parse(){

    libxl::IBookT<char> *localXlCreateBookA = xlCreateXMLBook();
    localXlCreateBookA->load(mPath.c_str());

    int count = localXlCreateBookA->sheetCount();
    int index = 0;
    for(int i = 0; i< count; i++){
        const char *sheetName = localXlCreateBookA->getSheetName(i);
        if(strcmp(sheetName, "matrix") == 0){
            index = i;
            break;
        }
    }
    libxl::ISheetT<char> *sheet = localXlCreateBookA->getSheet(index);

    for(int i  = 1; i < sheet->lastFilledRow(); i++){
        Msg msg;
        msg.channel = SPI;
        string signame = sheet->readStr(i,1);
        msg.name = Utils::trim(signame);

        //printf("parse file: %s\n", msg.name.c_str());

        string sender = sheet->readStr(i, 3);
        msg.permission =  strcmp(Utils::trim(sender).c_str(), "MCU") == 0 ?  PERMISSION::WRITE : PERMISSION::READ;


        string senderType = sheet->readStr(i, 5);
        msg.type = Utils::trim(senderType);

        /*
        string  cycleTime  = sheet->readStr(i, 6);
        msg.interval = toInt(Utils::trim(cycleTime));
        */


        string sigID = sheet->readStr(i, 7);
        msg.id = strtol(Utils::trim(sigID).c_str(), NULL,  16);
//        string type = sheet->readStr(i, 10);
//        type = Utils::trim(type);
        string sigLen = sheet->readStr(i, 8);

        msg.dataBytes = atol(Utils::trim(sigLen).c_str());
        canFrame[msg.id] = msg;

    }

    //printf("parse file: %d %d\n", hsize, vsize);

    this->parsed =  true;
    return true;
}
bool MDBFile::isParse(){
    return this->parsed;
}
unordered_map<uint16_t, Msg>& MDBFile::getMsg(){
    return canFrame;
}
std::string MDBFile::getChannel(){
    return  SPI;
}
