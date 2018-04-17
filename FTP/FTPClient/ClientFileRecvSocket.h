//
// Created by active on 18-4-18.
//

#ifndef NETWORKPROGRAMMING_CLIENTFILERECVSOCKET_H
#define NETWORKPROGRAMMING_CLIENTFILERECVSOCKET_H

#include "../../Socket/Socket.h"

struct FileInfo{
    char mode;
    char ID;
    char fileNameLen;
    char fileName[fileNameLen];
    int  fileLen;
};

struct FileStatus{
    char ID;
    bool status;
};

class ClientFileRecvSocket {

private:

    FileInfo fInfo;
    FileStatus fStatus;
    Socket sSocket;

public:

    ClientFileRecvSocket(){}
    ClientFileRecvSocket(Socket s){}
    int execute(){}
    int setFileInfo(char *fi){}
    FileInfo getFileInfo(){}
    int setFileStatus(){}
    FileStatus getFileStatus(){}
};


#endif //NETWORKPROGRAMMING_CLIENTFILERECVSOCKET_H
