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
    int i,j,k;
    k = 0;
    char data[255];
    ServerSocket serverSocket(port);
    serverSocket.listen();
    while(1){
        Socket clientSocket = serverSocket.accept();
        if(fork()==0){
            serverSocket.close();
            while(1){
                i = clientSocket.recv(data,255);
                cout<<"recv:"<<data<<endl;
                j = clientSocket.send(data,255);
                cout<<"send:"<<data<<endl;
                if(i>0&&j>0){
                    break;
                }
            }
            clientSocket.close();
            exit(1);
        }
        clientSocket.close();
        k++;
        if(k>2){
            break;
        }
    }
    serverSocket.close();
}

int TcpFork::excute() {

}
