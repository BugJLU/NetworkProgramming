//
// Created by mac on 2018/4/16.
//

#include <iostream>
#include "../Socket/Socket.h"

using namespace std;

#define SERV_PORT 12343
#define SERV_ADDR "127.0.0.1"

int main() {
    InetAddr addr = InetAddr();
    addr.setIP(SERV_ADDR);
    addr.setPort(SERV_PORT);
    Socket sock = Socket();
    int suc = sock.connect(addr);
    cout<<suc<<endl;

//    pthread_t pt = pthread_self();
    long int pt = time(0);
    char bufs[255], bufr[255];
    sprintf(bufs, "%ld", pt);
    cout<<bufs<<endl;
    while (1) {
        sock.send(bufs, sizeof(bufs));
        cout<<"send"<<bufs<<endl;
        sock.recv(bufr, 255);
        cout<<"recv"<<bufr<<endl;
    }
    sock.close();
    return 0;
}