//
// Created by mac on 2018/4/15.
//

#ifndef NETWORKPROGRAMMING_MULTISOCKET_H
#define NETWORKPROGRAMMING_MULTISOCKET_H

#include <vector>
#include "../Socket.h"

#define DEFAULT_SOCKNUM 1024

class MultiSocket {

    void init(int len) {
        maxNum = len;
        sockArr = new Socket[len];
        sockNum = 0;
    }

    Socket* sockArr;
    int sockNum;
    int maxNum;

public:
    MultiSocket(int sockNum = DEFAULT_SOCKNUM) {
        init(sockNum);
    }

    MultiSocket(const std::vector<Socket>& socks);

    // Return number of empty slots for socket, -1 means already full.
    int addSocket(const Socket &sock);

    void listenAll(std::vector<Socket>& inSocks, std::vector<Socket>& outSocks);
};


#endif //NETWORKPROGRAMMING_MULTISOCKET_H
