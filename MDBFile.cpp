#include "MDBFile.h"
#include "xls.h"
#include <stdio.h>
#include <stdlib.h>
MDBFile::MDBFile(string path)
{

    this->mPath = path;
}
string MDBFile::getPath(){
    return this->mPath;
}
bool MDBFile::parse(){
    xls::xlsWorkBook *book;
    xls::xlsWorkSheet *sheet;
    xls::xlsCell *cell;
    book = xls::xls_open(mPath.c_str(), "UTF-8");


    xls::xls_parseWorkBook(book);

    for(int index = 0; index < book->sheets.count; index++){

       sheet = xls::xls_getWorkSheet(book, index);
        xls::xls_parseWorkSheet(sheet);
        if(strcmp(book->sheets.sheet[index].name, "matrix") == 0){
            break;
        }
    }
    Msg lastMsg;
    for(int i=2; i< sheet->rows.lastrow; i++){
        Signal sig;
        cell = xls::xls_cell(sheet, i, 1);
        string msgName = cell->str;
        if(!msgName.empty()){
            Msg msg;
            msg.channel = SPI;

            string sender = xls::xls_cell(sheet, i, 3)->str;
            string senderType = xls::xls_cell(sheet, i, 5)->str;
            string msgID = xls::xls_cell(sheet, i, 7)->str;
            string msgLen = xls::xls_cell(sheet, i,8)->str;

            string sigName = xls::xls_cell(sheet, i, 11)->str;
            string sigStartByte =  xls::xls_cell(sheet, i, 9)->str;
            string sigLength =  xls::xls_cell(sheet, i, 10)->str;

            msg.name = Utils::trim(msgName);
            msg.permission = strcmp(Utils::trim(sender).c_str(), "MCU") == 0 ?  PERMISSION::WRITE : PERMISSION::READ;
            msg.type = Utils::trim(senderType);
            msg.id = strtol(Utils::trim(msgID).c_str(), NULL,  16);
            msg.dataBytes = atol(Utils::trim(msgLen).c_str());
            canFrame[msg.id] = msg;
            lastMsg = msg;


            sig.sigName = sigName;
            sig.startByte = atoi(sigStartByte.c_str());
            sig.sigSize = atoi(sigLength.c_str());
            canFrame[msg.id].sig[sigName] = sig;


         } else {
            string sigStartByte =  xls::xls_cell(sheet, i, 9)->str;
            string sigLength =  xls::xls_cell(sheet, i, 10)->str;
            string sigName;
            xls::xlsCell *nameSuffix = xls::xls_cell(sheet, i, 11);
            if(nameSuffix && nameSuffix->str){
                sigName = nameSuffix->str;
            }else{
                sigName = lastMsg.name+"::" + sigStartByte;
            }
            sig.sigName = sigName;
            sig.startByte = atoi(sigStartByte.c_str());
            sig.sigSize = atoi(sigLength.c_str());
            canFrame[lastMsg.id].sig[sigName] = sig;
         }

    }
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
