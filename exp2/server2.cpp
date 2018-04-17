//
// Created by leon on 18-4-15.
//

#include <cstring>
#include <pthread.h>
#include "../Socket/ServerSocket.h"

void* handle_client(void* arg);

const int MSG_SIZE = 30;
pthread_mutex_t mutex;
//static bool status = true;

int main()
{
    ServerSocket monitorSock;
    if(monitorSock.bind(9001) == -1)
    {
        puts("bin error!");
    }
//    monitorSock.close();
//    if(monitorSock.bind(9000) == -1)
//    {
//        puts("bin error!");
//    }
    monitorSock.listen();

    pthread_t handle_thread;


    pthread_mutex_init(&mutex,NULL);
    while(1)
    {
        Socket* clientSock = new Socket();
        puts("0");
        *clientSock = monitorSock.accept();
        puts("1");
        pthread_create(&handle_thread, NULL, handle_client, clientSock);
        puts("2");
        //pthread_join(handle_thread,NULL);            //如何实现不join能够实现线程通信
        //puts("3");
        //clientSock.close();
    }
    return 0;
}

void* handle_client(void* arg)
{
    char msg[MSG_SIZE];
    pthread_mutex_lock(&mutex);
    Socket* client = (Socket*)arg;
    fputs(msg, stdout);
    puts("bf recv");
    client->recv(msg,MSG_SIZE);
    puts("af recv");
    time_t t = time(0);
    strftime(msg,sizeof(msg),"%Y/%m/%d %X %A",localtime(&t));
    puts("bf send");
    client->send(msg, strlen(msg));
    puts("af send");
    pthread_mutex_unlock(&mutex);
    pthread_detach(pthread_self());
    client->close();
    delete client;
    return NULL;
}

