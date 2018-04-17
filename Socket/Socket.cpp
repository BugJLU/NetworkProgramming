//
// Created by mac on 2018/4/14.
//

#include <unistd.h>
#include "Socket.h"

class socketAssist {
public:
    static int _connect(int arg1, const struct sockaddr * arg2, socklen_t arg3) {
        return connect(arg1, arg2, arg3);
    }
    static ssize_t _send(int arg1, const void * arg2, size_t arg3, int arg4) {
        return send(arg1, arg2, arg3, arg4);
    }
    static ssize_t _recv(int arg1, void * arg2, size_t arg3, int arg4) {
        return recv(arg1, arg2, arg3, arg4);
    }
    static int _close(int arg1) {
        return close(arg1);
    }
};

int Socket::init() {
    cliSock = socket(AF_INET, SOCK_STREAM, 0);
    return cliSock;
}

Socket::Socket() {
    if (init() < 0) {
        throw "socket init error";
    }
}

int Socket::connect(InetAddr addr) {
    return socketAssist::_connect(cliSock, (struct sockaddr *)&addr.getAddress(), sizeof(struct sockaddr));
}

int Socket::send(const void* data, size_t len) {
    return static_cast<int>(socketAssist::_send(cliSock, data, len, 0));
}

int Socket::recv(void *data, size_t len) {
    return static_cast<int>(socketAssist::_recv(cliSock, data, len, 0));
}

int Socket::close() {
    return socketAssist::_close(cliSock);
}




//Socket::Socket(InetAddr addr) {
//    if (init() < 0) {
//        throw "socket init error";
//    }
//
//}

