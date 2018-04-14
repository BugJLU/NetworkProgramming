//
// Created by mac on 2018/4/14.
//

#ifndef NETWORKPROGRAMMING_SERVERSOCKET_H
#define NETWORKPROGRAMMING_SERVERSOCKET_H

#define SER_BACKLOG 20

#include "Socket.h"

class ServerSocket {

    int serSock;

    int init();

public:
    ServerSocket();
    ServerSocket(int port);
    int bind(int port);
    int listen();
    Socket& accept();
    int close();
};


#endif //NETWORKPROGRAMMING_SERVERSOCKET_H
