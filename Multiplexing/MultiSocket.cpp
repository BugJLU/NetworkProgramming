//
// Created by mac on 2018/4/15.
//

#include <poll.h>
#include "MultiSocket.h"

MultiSocket::MultiSocket(const std::vector<Socket> &socks) {
    init(static_cast<int>(socks.size()));
    for (int i = 0; i < socks.size(); ++i) {
        addSocket(socks[i]);
    }
}

int MultiSocket::addSocket(const Socket &sock) {
    if (sockNum == maxNum)
        return -1;
    sockArr[sockNum] = sock;
    sockNum++;
    return maxNum - sockNum;
}

void MultiSocket::listenAll(std::vector<Socket> &inSocks, std::vector<Socket> &outSocks) {
    struct pollfd sockpolls[sockNum];
    int max = -1;
    for (int i = 0; i < sockNum; ++i) {
        sockpolls[i].fd = sockArr[i].getFd();
        sockpolls[i].events = POLLIN | POLLOUT;
        if (sockpolls[i].fd > max)
            max = sockpolls[i].fd;
    }
    max += 1;

    int res = poll(sockpolls, max, 0);

    if (res < 0) {
        throw "poll error";
    }
    if (res == 0) {
        return;
    }

    // Some sockets are ready to write/read.
    for (int i = 0; i < sockNum; ++i) {
        if (sockpolls[i].revents & POLLIN) {
            inSocks.push_back(sockArr[i]);
        }
        if (sockpolls[i].revents & POLLOUT) {
            inSocks.push_back(sockArr[i]);
        }
    }
}
