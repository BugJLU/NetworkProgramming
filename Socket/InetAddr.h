//
// Created by mac on 2018/4/14.
//

#ifndef NETWORKPROGRAMMING_INETADDR_H
#define NETWORKPROGRAMMING_INETADDR_H


#include <netinet/in.h>
#include <string>

class InetAddr {
    struct sockaddr_in addr;
public:
    InetAddr();
    void setIP(std::string ip);
    void setPort(int port);
    struct sockaddr_in & getAddress();
};


#endif //NETWORKPROGRAMMING_INETADDR_H
