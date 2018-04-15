//
// Created by leon on 18-4-15.
//

#include <cstring>
#include "ServerSocket.h"

void* send_msg(void* arg);
void* recv_msg(void* arg);

const int MSG_SIZE = 30;

int main()
{
    ServerSocket monitorSock;
    monitorSock.bind(8000);
    monitorSock.listen();

    pthread_t recv_thread;
    pthread_t send_thread;

    Socket clientSock;
    while(1)
    {
        clientSock = monitorSock.accept();
        puts("1");
        pthread_create(&recv_thread, NULL, recv_msg, &clientSock);
        puts("2");
        pthread_create(&send_thread, NULL, send_msg, &clientSock);
        puts("3");
        pthread_join(recv_thread, NULL);
        pthread_join(send_thread, NULL);

    }
}

void* send_msg(void* arg)
{
    char msg[MSG_SIZE];
    Socket client = *(Socket*)arg;
    time_t t = time(0);
    strftime(msg,sizeof(msg),"%Y/%m/%d %X %A 本年第%j天 %z",localtime(&t));
    client.send(msg, strlen(msg));
    return NULL;

}

void* recv_msg(void* arg)
{
    char msg[MSG_SIZE];
    Socket client = *(Socket*)arg;
    client.recv(msg, MSG_SIZE);
    fputs(msg, stdout);
    return NULL;
}
