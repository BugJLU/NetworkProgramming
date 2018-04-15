//
// Created by leon on 18-4-15.
//

#include <cstring>
#include "InetAddr.h"
#include "Socket.h"

void* send_msg(void* arg);
void* recv_msg(void* arg);

const int MSG_SIZE = 30;
static int status = 1; //循环标志位

int main()
{
    InetAddr address;
    address.setIP("127.0.0.1");
    address.setPort(8000);

    Socket clientSock;
    if( clientSock.connect(address) == -1) {
        puts("connect error!");
        return 0;
    }

    pthread_t send_thread;
    pthread_t recv_thread;

    while(status)
    {
        pthread_create(&send_thread, NULL, send_msg, &clientSock);
        puts("1");
        pthread_create(&recv_thread, NULL, recv_msg, &clientSock);
        puts("2");
        pthread_join(send_thread, NULL);
        puts("3");
        pthread_join(recv_thread, NULL);
        puts("4");
    }
    return 0;
}

void* send_msg(void* arg)
{
    char msg[MSG_SIZE];
    Socket client = *(Socket*)arg;
    fgets(msg, strlen(msg), stdin);
    puts(msg);
    if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        status = 0;
    else
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