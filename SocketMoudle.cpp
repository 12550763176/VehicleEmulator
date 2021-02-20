#include "SocketMoudle.h"

#include <qmessagebox.h>


SocketMoudle::SocketMoudle() {
//    sprintf(startCmd, "adb forward tcp:%d tcp:%d", localProt, remotePort);
//    sprintf(stopCmd, "adb forward --remove tcp:%d", localProt);
//    sprintf(listCmd, "adb forward --list");
    sprintf(listDev, "adb devices");

}
SocketMoudle::~SocketMoudle(){

}

void SocketMoudle::selectDevice(string id){
    deviceID = id;
    //sprintf(listCmd, "adb -s %s forward --list", deviceID.c_str());
    sprintf(stopCmd, "adb -s %s forward --remove tcp:%d ; adb -s %s forward --remove tcp:%d", deviceID.c_str(), localCanProt, deviceID.c_str(), localSpiProt);
    sprintf(startCmd, "adb -s %s forward tcp:%d tcp:%d  ; adb -s %s forward tcp:%d tcp:%d ", deviceID.c_str(), localCanProt, remoteCanPort, deviceID.c_str(), localSpiProt, remoteSpiPort);
}

int SocketMoudle::closeFD(string type){


}

SocketInfo& SocketMoudle::stopSocket(){
    static SocketInfo info;

    if(socketCAN_FD == 0){
        printf("can already stop\n");
        info.can_fd = socketCAN_FD;
        info.error = -1;
        info.str = "can socket already start;";
    }
    if(socketSPI_FD == 0){
        printf("SPI already stop\n");
        info.spi_fd = socketSPI_FD;
        info.error = -1;
        info.str += "spi socket already start;";
    }
    if(socketSPI_FD == 0 && socketCAN_FD == 0){return info;}

    uint32_t err = close(socketCAN_FD);
    if(err != 0 ){
        info.can_fd = socketCAN_FD;
        info.error = err;
        info.str += "CAN close failed;";
    } else {
        socketCAN_FD = 0;
    }

    err = close(socketSPI_FD);
    if(err != 0 ){
        info.spi_fd = socketSPI_FD;
        info.error = err;
        info.str += "spi close failed;";
    } else {
        socketSPI_FD = 0;
    }

    err = system(stopCmd);
    if(err != 0 ){
        info.error = err;
        info.str += strcat(stopCmd, " exec failed");
    } else {
        info.error = err;
        info.str += "close Success";
    }
    return info;
}

bool SocketMoudle::isConnect(string type){
    return  type == SPI ? socketSPI_FD != 0 : socketCAN_FD != 0;
}
int SocketMoudle::createFD(string type){
    int socketFD  = socket(AF_INET, SOCK_STREAM, 6);
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(type == SPI ? localSpiProt : localCanProt);

    uint32_t code = ::connect(socketFD, (struct sockaddr*) &servAddr, sizeof(sockaddr_in));
    if(socketFD >= 0 && code >= 0) {
        return socketFD;
    } else {
         int tmp = close(socketFD);
         printf("create %s fd failed, error code %d\n", type.c_str(), errno);
         return 0;
    }
}

SocketInfo& SocketMoudle::startSocket(){
    static SocketInfo info;
    uint32_t err = system(startCmd);
    if(err != 0) {
        info.error = err;
        info.str = strcat(startCmd, " exec failed");
        return info;
    }

    if(socketCAN_FD != 0){
        printf("already start\n");
        info.can_fd = socketCAN_FD;
        info.error = -1;
        info.str = "socket already start";
        return info;
    }else {
        socketCAN_FD = createFD(CAN);
        info.can_fd = socketCAN_FD;

    }
    if(socketSPI_FD != 0){
        printf("already start\n");
        info.spi_fd = socketSPI_FD;
        info.error = -1;
        info.str = "socket already start";
        return info;
    }else {
        socketSPI_FD= createFD(SPI);
        info.spi_fd = socketSPI_FD;
    }

    return info;
}

vector<string> SocketMoudle::split(string str, string pattern){
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

string SocketMoudle::getDeviceId()
{
    return deviceID;
}


vector<string> SocketMoudle::listDevices(){
    vector<string> vec;
    char buf[1024];
    FILE *ptr;
    if((ptr = popen(listDev, "r")) != NULL){
        while(fgets(buf, 1024, ptr)!=NULL)  {
            vector<string> tmp = split(buf, "\t");
            if(tmp.size() == 2){
                vec.push_back(tmp[0]);
            }
        }
        pclose(ptr);
        ptr = NULL;
    } else {
        printf("popen error\n");
    }
    return vec;
}


SocketInfo& SocketMoudle::write(string type,  std::vector<uint8_t>& data){
    //for(int i = 0; i < data.size(); i++)
    static SocketInfo info;
    //printf("SocketMoudle::write  %s \n\n", type.c_str());
    uint32_t err;
    if(type == SPI){
        if(socketSPI_FD == 0){
            printf("error\n");
            QMessageBox::about(NULL, "About", "send Data failed, socket error");
        }
        err = ::write(socketSPI_FD, data.data(), data.size());
    } else if(type == CAN){
        if(socketCAN_FD ==0){
            printf("error\n");
            QMessageBox::about(NULL, "About", "send Data failed, socket error");
        }



        err = ::write(socketCAN_FD, data.data(), data.size());
    }
//    if(err != 0){
//        info.fd = socketFD;
//        info.error = err;
//        info.str = "write data error";
        //QMessageBox::about(NULL, "About", "send Data failed");
        //stopSocket();
//    }

    return info;
}


std::vector<uint8_t> SocketMoudle::read(){
    return std::vector<uint8_t>();
}

