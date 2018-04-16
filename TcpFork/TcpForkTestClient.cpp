//
// Created by active on 18-4-16.
//
#include <iostream>
#include "../ServerSocket.h"

using namespace std;

int main() {
    cout<<"SocketTest:"<<endl;

    InetAddr addr;
    addr.setIP("127.0.0.1");
    addr.setPort(12343);
    Socket clientSocket;
    clientSocket.connect(addr);
    int k = 0;
    while (true){
        string data;
        cin>>data;
        clientSocket.send(data.data(), data.length());
        char data1[255];
        clientSocket.recv(data1, 255);
        cout<<data1<<endl;
        k++;
        if (k>3){
            break;
        }
    }

    clientSocket.close();

    return 0;
}