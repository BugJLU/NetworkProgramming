//
// Created by leon on 18-4-15.
//

#include <cstring>
#include <zconf.h>
#include <iostream>
#include "../InetAddr.h"
#include "../Socket.h"
using namespace std;

void* send_msg(void* arg);
void* recv_msg(void* arg);

const int MSG_SIZE = 30;
static int status = 1; //循环标志位

int main()
{
    InetAddr address;
    address.setIP("127.0.0.1");
    address.setPort(9001);



    pthread_t send_thread;
    pthread_t recv_thread;



    while(status)
    {
        Socket clientSock;


        if( clientSock.connect(address) == -1) {
            puts("connect error!");
            return 0;
        }
        pthread_create(&send_thread, NULL, send_msg, &clientSock);
        pthread_create(&recv_thread, NULL, recv_msg, &clientSock);
        pthread_join(send_thread, NULL);
        pthread_join(recv_thread, NULL);
        clientSock.close();
    }
    return 0;
}

void* send_msg(void* arg)
{
    puts("bf send");
    char msg[MSG_SIZE];
    Socket client = *(Socket*)arg;
    cout << strlen(msg) << endl;
    fgets(msg, MSG_SIZE, stdin);            //为啥MSG_SIZE就可以，而strlen(msg)就不行

    fputs(msg, stdout);
    if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        status = 0;
    else
        client.send(msg, strlen(msg));
    puts("af send");
    return NULL;

}

void* recv_msg(void* arg)
{
    puts("bf recv");
    char msg[MSG_SIZE];
    Socket client = *(Socket*)arg;
    client.recv(msg, MSG_SIZE);
    fputs(msg, stdout);
    puts("af recv");
    return NULL;
}