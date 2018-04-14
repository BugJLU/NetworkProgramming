//
// Created by mac on 2018/4/14.
//

#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "InetAddr.h"

InetAddr::InetAddr() {
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
}

struct sockaddr_in &InetAddr::getAddress() {
    return addr;
}

void InetAddr::setIP(std::string ip) {
//    gethostbyname(ip.data());
    addr.sin_addr.s_addr = inet_addr(ip.data());
}

void InetAddr::setPort(int port) {
    addr.sin_port = htons(port);
}
