//
// Created by blacksun on 18-4-18.
//

#ifndef NETWORKPROGRAMMING_FTPCLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <queue>

#define DATA_PORT 2325
#define SERV_PORT 7777
#define ERROR -1

typedef struct file_recv_ans{
    unsigned char id;
    char filename[256];
    int status;
} FileRecvANS;

class FtpClient{
public:
    FtpClient(char*server);
    void start();
private:
    static std::queue<FileRecvANS> FRAqueue;
    struct hostent* server;
    static void* processCmd(void * arg);
    static void* processData(void * arg);
    static void* processTrans(void * arg);


};

#endif //NETWORKPROGRAMMING_FTPCLIENT_H
