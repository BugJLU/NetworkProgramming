//
// Created by active on 18-4-15.
//
#include <unistd.h>
#include <iostream>
#include "TcpFork.h"
#include "../Socket.h"
#include "../ServerSocket.h"

using namespace std;



TcpFork::TcpFork(int port) {
    this->port = port;
}

int TcpFork::execute() {
    int i,j,k;
    k = 0;
    char data[255];
    ServerSocket serverSocket(port);
    serverSocket.listen();
    while(true){
        Socket clientSocket = serverSocket.accept();
        if(fork()==0){
            serverSocket.close();
            while(true){
                i = clientSocket.recv(data,255);
                cout<<k<<"->recv:"<<data<<endl;
                j = clientSocket.send(data,255);
                cout<<k<<"->send:"<<data<<endl;
                if(i<0){
                    break;
                }
            }
            clientSocket.close();
            exit(1);
        }
        clientSocket.close();
        k++;
        if(k>9){
            break;
        }
    }
    serverSocket.close();
}
