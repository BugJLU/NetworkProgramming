//
// Created by mac on 2018/4/14.
//

#include "ServerSocket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

class serSocketAssist {
public:
    static int _bind(int arg1, const struct sockaddr * arg2, socklen_t arg3) {
        return bind(arg1, arg2, arg3);
    }
    static int _listen(int arg1, int arg2) {
        return listen(arg1, arg2);
    }
    static int _accept(int arg1, struct sockaddr * __restrict arg2, socklen_t * __restrict arg3) {
        return accept(arg1, arg2, arg3);
    }
    static int _close(int arg1) {
        return close(arg1);
    }
};

ServerSocket::ServerSocket() {
    if (init() < 0) {
        throw "socket init error";
    }
}

ServerSocket::ServerSocket(int port) {
    if (init() < 0) {
        throw "socket init error";
    }
    bind(port);
}

int ServerSocket::bind(int port) {
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    serSocketAssist::_bind(serSock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    return 0;
}

int ServerSocket::listen() {
    return serSocketAssist::_listen(serSock, SER_BACKLOG);
}

Socket& ServerSocket::accept() {
    int cliSock;
    struct sockaddr_in cliAddr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    cliSock = serSocketAssist::_accept(serSock, (struct sockaddr *)&cliAddr, &addrlen);
    if (cliSock < 0) {
        throw "socket accept error";
    }
    Socket *cliSocket = new Socket();
    //TODO: to be finished.
    return *cliSocket;
}

int ServerSocket::close() {
    return serSocketAssist::_close(serSock);
}

int ServerSocket::init() {
    serSock = socket(AF_INET, SOCK_STREAM, 0);
    return serSock;
}
