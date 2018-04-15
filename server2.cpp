//
// Created by leon on 18-4-15.
//

#include <cstring>
#include "ServerSocket.h"

void* handle_client(void* arg)

const int MSG_SIZE = 30;
pthread_mutex_t mutex;

int main()
{
    ServerSocket monitorSock;
    monitorSock.bind(8000);
    monitorSock.listen();

    pthread_t handle_thread;

    Socket clientSock;
    pthread_mutex_init(&mutex,NULL);
    while(1)
    {
        clientSock = monitorSock.accept();
        puts("1");
        pthread_create(&handle_thread, NULL, handle_client, &clientSock);
        puts("2");
        pthread_detach(handle_thread)

    }
}

void* handle_client(void* arg)
{
    char msg[MSG_SIZE];
    pthread_mutex_lock(&mutex);
    Socket client = *(Socket*)arg;
    time_t t = time(0);
    strftime(msg,sizeof(msg),"%Y/%m/%d %X %A",localtime(&t));
    fputs(msg, stdout);
    client.send(msg, strlen(msg));
    pthread_mutex_lock(&mutex);
    return NULL;
}

