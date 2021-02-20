#ifndef SOCKETMOUDLE_H
#define SOCKETMOUDLE_H

#include <mutex>
#include <vector>
#include <string>
#include <string.h>
#include <list>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "MsgFile.h"
using namespace std;


struct SocketInfo {
    uint32_t error = 0;
    uint32_t spi_fd = 0;
    uint32_t can_fd = 0;
    string str;
};


class SocketMoudle{
private:
    int32_t localCanProt = 33453;
    int32_t remoteCanPort = 33452;

    int32_t localSpiProt = 33454;
    int32_t remoteSpiPort = 33451;

    string deviceID;
    int32_t socketSPI_FD;
    int32_t socketCAN_FD;

    SocketMoudle();
    ~SocketMoudle();

    //SocketMoudle& operator=(const SocketMoudle&) = delete;
    SocketMoudle(const SocketMoudle&) = delete;
    std::vector<string> split(string str, string pattern);

public:
    char startCmd[200] ;

    char stopCmd[200] ;

    char listCmd[200] ;
    char listDev[200] ;
    //int mState;

    string getDeviceId();
    bool isConnect(string type);
    SocketInfo& startSocket();
    SocketInfo& stopSocket();

    std::vector<uint8_t> read();
    SocketInfo& write(string type, std::vector<uint8_t>& vec);

    int createFD(string type);
    int closeFD(string type);

    void selectDevice(string id);
    vector<string> listDevices();

    static SocketMoudle& getInstance() {
        static SocketMoudle instance;
        return instance;
    }
};

#endif // SOCKETMOUDLE_H
