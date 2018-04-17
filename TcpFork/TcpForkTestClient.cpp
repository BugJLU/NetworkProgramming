//
// Created by active on 18-4-16.
//
#include <iostream>
#include "../Socket/ServerSocket.h"

using namespace std;

int main() {
    cout<<"TcpForkTestClient:"<<endl;

    InetAddr addr;
    addr.setIP("127.0.0.1");
    addr.setPort(12343);
    Socket clientSocket;
    clientSocket.connect(addr);
    int k = 0;
    while (true){
        string data;
        cout<<"cin:";
        cin>>data;
        clientSocket.send(data.data(), data.length());
        cout<<"ClientSend:"<<data<<endl;
        char data1[255];
        clientSocket.recv(data1, 255);
        cout<<"ClientRecv:"<<data1<<endl;
        k++;
        if (k>3){
            break;
        }
    }

    clientSocket.close();

    return 0;
}