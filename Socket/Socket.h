//
// Created by mac on 2018/4/14.
//

#ifndef NETWORKPROGRAMMING_SOCKET_H
#define NETWORKPROGRAMMING_SOCKET_H


#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "InetAddr.h"

class Socket {
    int cliSock;
    int init();
public:
    friend class ServerSocket;
    Socket();
//    Socket(InetAddr addr);
    int getFd() {
        return cliSock;
    }
    void setFd(int sockfd);
    InetAddr& getPeerAddr();
    int connect(InetAddr addr);
    int send(const void* data, size_t len);
    int recv(void* data, size_t len);
    int close();

};


#endif //NETWORKPROGRAMMING_SOCKET_H
