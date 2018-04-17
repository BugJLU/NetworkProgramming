//
// Created by mac on 2018/4/15.
//

#include <iostream>
#include "Socket/ServerSocket.h"

using namespace std;

int main() {
    ServerSocket serverSocket(12343);
    serverSocket.listen();
    Socket client = serverSocket.accept();
    char data[255];
    client.recv(data, 255);
    cout<<data<<endl;
    cin>> data;
    client.send(data, sizeof(data));
    client.close();
    serverSocket.close();
    
}