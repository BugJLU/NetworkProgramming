//
// Created by active on 18-4-18.
//

#ifndef NETWORKPROGRAMMING_CLIENTFILERECVSOCKET_H
#define NETWORKPROGRAMMING_CLIENTFILERECVSOCKET_H

#include "../../Socket/Socket.h"
#include "../../FileOperation/File.h"
#include <vector>
#include <mutex>
#include <iostream>

#define BUFFER_LENGTH 256

struct FileInfo{
    char mode;
    char ID;
    char fileNameLen;
    char *fileName;
    uint32_t fileLen;
};

struct FileStatus{
    char ID;
    char *fileName;
    bool status;
};

typedef struct {
    Socket serverSocket;
    std::vector<FileStatus> *fsQueue;
    pthread_mutex_t *qMutex;

}Info;


class ClientFileRecvSocket {

private:

    FileInfo fInfo;
    FileStatus fStatus;
    Info cInfo;

public:

    ClientFileRecvSocket();
    ClientFileRecvSocket(Info *ci);
    int execute();
    int setFileInfo();
    int setFileStatus(int s);
    int setQueue();
};

void* ClientFileRecv(void* arg);

#endif //NETWORKPROGRAMMING_CLIENTFILERECVSOCKET_H
