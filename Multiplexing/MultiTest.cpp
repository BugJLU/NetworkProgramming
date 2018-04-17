//
// Created by mac on 2018/4/16.
//

//
//                                  _oo8oo_
//                                 o8888888o
//                                 88" . "88
//                                 (| -_- |)
//                                 0\  =  /0
//                               ___/'==='\___
//                             .' \\|     |// '.
//                            / \\|||  :  |||// \
//                           / _||||| -:- |||||_ \
//                          |   | \\\  -  /// |   |
//                          | \_|  ''\---/''  |_/ |
//                          \  .-\__  '-'  __/-.  /
//                        ___'. .'  /--.--\  '. .'___
//                     ."" '<  '.___\_<|>_/___.'  >' "".
//                    | | :  `- \`.:`\ _ /`:.`/ -`  : | |
//                    \  \ `-.   \_ __\ /__ _/   .-` /  /
//                =====`-.____`.___ \_____/ ___.`____.-`=====
//                                  `=---=`
//

#include <iostream>
#include <unistd.h>
#include "MultiSocket.h"
#include "../ServerSocket.h"

using namespace std;

bool flag;
MultiSocket multiSocket;
pthread_mutex_t mux;

typedef struct ListenArg {
    ServerSocket* sock;
    MultiSocket* multi;
}listenArg;

void* serlisten(void* arg) {
    listenArg* larg = (listenArg*)arg;
    ServerSocket* server = (larg->sock);
    MultiSocket* multiSocket = (larg->multi);
    server->listen();
    pthread_detach(pthread_self());

    while(flag) {
        Socket tmp = server->accept();

        pthread_mutex_lock(&mux);
        multiSocket->addSocket(tmp);
        pthread_mutex_unlock(&mux);

    }

    cout<<"----------end----------";
}

int main() {
    pthread_mutex_init(&mux, NULL);
    ServerSocket server(12343);
    multiSocket = MultiSocket();
    listenArg larg = {
            &server,
            &multiSocket
    };
    flag = true;
    pthread_t pt;
    pthread_create(&pt, NULL, serlisten, (void*)&larg);

    vector<Socket> in = vector<Socket>();
    vector<Socket> out = vector<Socket>();
    char buf[255];
    while(flag) {
        in.clear();
        out.clear();
        pthread_mutex_lock(&mux);
//        if (multiSocket.getSocks().size() == 0) {
//            sleep(1);
//            pthread_mutex_unlock(&mux);
//            continue;
//        }
        try {

            multiSocket.listenAll(in, out);
//            cout<<multiSocket.getSocks().size()<<" "<<in.size()<<" "<<out.size()<<endl;
//            sleep(1);
            pthread_mutex_unlock(&mux);
        } catch (char* str) {
            cout<<str;
        }
        for (int i = 0; i < in.size(); ++i) {
//            cout<<in[i].getFd()<<endl;
            in[i].recv(buf, 255);
//            cout<<"receive from "<<in[i].getFd()<<": "<<buf<<endl;
        }
        for (int i = 0; i < out.size(); ++i) {
            sprintf(buf, "%ld", out[i].getFd());
            out[i].send(buf, 11);
//            cout<<"send to "<<out[i].getFd()<<": hi"<<endl;
        }
    }

    flag = false;
    pthread_mutex_destroy(&mux);
    return 0;
}