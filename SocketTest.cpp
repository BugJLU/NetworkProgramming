//
// Created by mac on 2018/4/15.
//

#include <iostream>
#include "Socket/ServerSocket.h"

using namespace std;

int main() {
    cout<<"SocketTest:"<<endl;

    InetAddr addr;
    addr.setIP("127.0.0.1");
    addr.setPort(12343);
    Socket clientSocket;
    clientSocket.connect(addr);
    string data = "hello, world";
    clientSocket.send(data.data(), data.length());
    char data1[255];
    clientSocket.recv(data1, 255);
    cout<<data1<<endl;
    clientSocket.close();

    return 0;
}