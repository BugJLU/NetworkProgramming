//
// Created by active on 18-4-15.
//
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "TcpFork.h"
#include "../Socket.h"
#include "../ServerSocket.h"

using namespace std;



TcpFork::TcpFork(int port) {
    this->port = port;
}

int TcpFork::execute() {
    int recvlen,k;
    bool flag = false;
    k = 0;
    ServerSocket serverSocket(port);
    serverSocket.listen();
    while(true){
        Socket clientSocket = serverSocket.accept();
        k++;
        if(fork()==0){
            serverSocket.close();
            while(true){
                char data[255];
                memset(data,0, sizeof(data));
                recvlen = clientSocket.recv(data,255);
                cout<<k<<"->recv:"<<data<<endl;
                clientSocket.send(data,255);
                cout<<k<<"->send:"<<data<<endl;
                if(recvlen<0){
                    if (errno == EINTR){
                        continue;
                    }
                    else{
                        break;
                    }
                }else if (recvlen == 0){
                    break;
                }
            }
            clientSocket.close();
            exit(1);
        }
        clientSocket.close();
        if (k>3){
            flag = true;
        }
        if(flag){
            break;
        }
    }
    serverSocket.close();
}
