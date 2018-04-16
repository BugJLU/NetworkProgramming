//
// Created by mac on 2018/4/15.
//

#ifndef NETWORKPROGRAMMING_MULTISOCKET_H
#define NETWORKPROGRAMMING_MULTISOCKET_H

#include <vector>
#include <pthread.h>
#include "../Socket.h"

//#define DEFAULT_SOCKNUM 1024

class MultiSocket {

    void mutexInit() {
        pthread_mutex_init(&mutex, NULL);
    }

    std::vector<Socket> sockArr;
    pthread_mutex_t mutex;

public:
    MultiSocket() {
        mutexInit();
        sockArr = std::vector<Socket>();
    }

    MultiSocket(const std::vector<Socket>& socks);

    ~MultiSocket() {
        pthread_mutex_destroy(&mutex);
    }

    // ------ Return number of empty slots for socket, -1 means already full.
    int addSocket(const Socket &sock);

    std::vector<Socket> getSocks() {
        return sockArr;
    }

    void listenAll(std::vector<Socket>& inSocks, std::vector<Socket>& outSocks);
};


#endif //NETWORKPROGRAMMING_MULTISOCKET_H
