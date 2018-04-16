//
// Created by mac on 2018/4/15.
//

#include <poll.h>
#include "MultiSocket.h"

MultiSocket::MultiSocket(const std::vector<Socket> &socks) {
    mutexInit();
    sockArr = std::vector<Socket>(socks);
}

//int MultiSocket::addSocket(const Socket &sock) {
//    if (sockNum == maxNum)
//        return -1;
//    sockArr[sockNum] = sock;
//    sockNum++;
//    return maxNum - sockNum;
//}

int MultiSocket::addSocket(const Socket &sock) {
//    pthread_mutex_lock(&mutex);
    sockArr.push_back(sock);
//    pthread_mutex_unlock(&mutex);
}

typedef struct pollfd _pollfd;

void MultiSocket::listenAll(std::vector<Socket> &inSocks, std::vector<Socket> &outSocks) {
//    pthread_mutex_lock(&mutex);
    int max = -1;

    int asize = sockArr.size();

    _pollfd* sockpolls = new _pollfd[asize];

    for (int i = 0; i < asize; ++i) {
        sockpolls[i].fd = sockArr[i].getFd();
        sockpolls[i].events = POLLIN | POLLOUT;
        if (sockpolls[i].fd > max)
            max = sockpolls[i].fd;
    }
    max += 1;

    int res = poll(sockpolls, max, 0);
//    int res = 3;

    // TODO: try catch ??
//    if (res < 0) {
//        throw "poll error";
//    }
    if (res < 0) {
        delete [] sockpolls;
        return;
    }
    if (res == 0) {
        delete [] sockpolls;
        return;
    }

    // Some sockets are ready to write/read.
    for (int i = asize-1; i >=0; --i) {
        Socket sock1 = sockArr[i];
        //TODO
        if (sockpolls[i].revents & POLLIN) {
            inSocks.push_back(sock1);
        }
        if (sockpolls[i].revents & POLLOUT) {
            outSocks.push_back(sock1);
        }
        //TODO
    }
//    pthread_mutex_unlock(&mutex);
    delete [] sockpolls;
}

