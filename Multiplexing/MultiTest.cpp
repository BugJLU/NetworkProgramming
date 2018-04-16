//
// Created by mac on 2018/4/16.
//

#include <iostream>
#include <unistd.h>
#include "MultiSocket.h"
#include "../ServerSocket.h"

using namespace std;

bool flag;
MultiSocket multiSocket;
pthread_mutex_t mux;

void* serlisten(void* arg) {
    ServerSocket server = *(ServerSocket*)arg;
    server.listen();
    pthread_detach(pthread_self());

    while (flag) {
        Socket tmp;
        tmp = server.accept();

        pthread_mutex_lock(&mux);
        multiSocket.addSocket(tmp);
        sleep(1);
        pthread_mutex_unlock(&mux);

        sleep(1);
    }

    cout<<"----------end----------";
}

int main() {
    pthread_mutex_init(&mux, NULL);
    ServerSocket server(12343);
    multiSocket = MultiSocket();
    flag = true;
    pthread_t pt;
    pthread_create(&pt, NULL, serlisten, (void*)&server);

    vector<Socket> in = vector<Socket>();
    vector<Socket> out = vector<Socket>();
    char buf[255];
    for (int i = 0; i < 1000000; ++i) {
        in.clear();
        out.clear();
        try {
            pthread_mutex_lock(&mux);
            multiSocket.listenAll(in, out);
            cout<<multiSocket.getSocks().size()<<" "<<in.size()<<" "<<out.size()<<endl;
            sleep(1);
            pthread_mutex_unlock(&mux);
        } catch (char* str) {
            cout<<str;
        }
        for (int i = 0; i < in.size(); ++i) {
            cout<<in[i].getFd()<<endl;
            in[i].recv(buf, 255);
            cout<<"receive from "<<in[i].getFd()<<": "<<buf<<endl;
        }
        for (int i = 0; i < out.size(); ++i) {
            out[i].send("hi", 3);
            cout<<"send to "<<out[i].getFd()<<": hi"<<endl;
        }
        sleep(2);
    }

    flag = false;
    pthread_mutex_destroy(&mux);
    return 0;
}