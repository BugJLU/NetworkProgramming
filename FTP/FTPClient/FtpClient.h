//
// Created by blacksun on 18-4-18.
//

#ifndef NETWORKPROGRAMMING_FTPCLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include<vector>
#include "../../Socket/Socket.h"
#include "ClientFileRecvSocket.h"

#define DATA_PORT 2325
#define SERV_PORT 7777
#define ERROR -1

//struct FileInfo{
//    char mode;
//    char ID;
//    char fileNameLen;
//    char *fileName;
//    uint32_t fileLen;
//};
//
//struct FileStatus{
//    char ID;
//    char *fileName;
//    bool status;
//};
//
//typedef struct {
//    Socket serverSocket;
//    std::vector<FileStatus> *fsQueue;
//    pthread_mutex_t *qMutex;
//
//}Info;

class FtpClient{
public:
    FtpClient(char*server);
    void start();
private:
    static bool running;
    static bool __init;
    static bool init();
    static std::vector<FileStatus>* fsQueue;
    //static pthread_mutex_t* qMutex;
    struct hostent* server;
    static void* processCmd(void * arg);
    static void* processData(void * arg);
    static void* processTrans(void * arg);


};


#endif //NETWORKPROGRAMMING_FTPCLIENT_H
