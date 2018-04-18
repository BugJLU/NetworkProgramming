//
// Created by blacksun on 18-4-18.
//
#include"FtpClient.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

#define MAXACC 10

FtpClient::FtpClient(char*server) {
    this->server = gethostbyname(server);
}

void FtpClient::start() {

    pthread_t cmd_tid, data_tid;
    pthread_create(&cmd_tid,NULL,processCmd,(void*)server);
    pthread_create(&data_tid,NULL,processData,NULL);
}

void* FtpClient::processCmd(void *arg) {
    int sockfd, n;
    char send_buffer[261];
    char filename[256];
    unsigned char filelen;
    char cmd[10];
    struct sockaddr_in serv_addr;
    struct hostent * mserver  = (struct hostent *)arg;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
       perror("cmd:Error opening socket");
       exit(1);
    }

    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)mserver->h_addr_list[0],
          (char *)&serv_addr.sin_addr.s_addr,
          mserver->h_length);
    serv_addr.sin_port = htons(SERV_PORT);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        perror("cmd:ERROR connecting");
        exit(1);
    }
    unsigned char cmdcount = 0;
    while(true){
        printf("Please enter the command: ");
        scanf("%s",cmd);
        if(strcmp(cmd,"get")){
            scanf("%s",filename);
            send_buffer[0] = 0;
        }else{
            printf("unvalid command");
            break;
        }

        if((filelen = strlen(filename))>0){
            send_buffer[1] = cmdcount;
            send_buffer[2] = DATA_PORT>>8;
            send_buffer[3] = DATA_PORT & 0x00ff;
            send_buffer[4] = filelen;
            for(int i = 1;i<=filelen;i++){
                send_buffer[4+i] = filename[i-1];
            }
            send_buffer[5+filelen] = '\0';
            if((n = write(sockfd, send_buffer,strlen(send_buffer)))<0){
                perror("cmd:Error writing to socket");
                break;
            }

        }

    }
    close(sockfd);
    return 0;
}

void* FtpClient::processData(void *arg) {
    int sockfd, newsockfd, clicount;
    struct sockaddr_in serv_addr, cli_addr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("data:Error opening socket");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(DATA_PORT);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("data:Error on binding");
        exit(1);
    }
    listen(sockfd,MAXACC);
    socklen_t clilen = sizeof(cli_addr);
    while(true){
        if((newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen))<0){
            perror("data:Error on accept");
            break;
        }
        printf("accept a connect(addr:%s, port:%d)",
               inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
        pthread_t trans_tid;
        pthread_create(&trans_tid,NULL,processTrans,(void*)newsockfd);
    }
    close(sockfd);
    return 0;
}

void* FtpClient::processTrans(void *arg) {
    int newsockfd = *((int*)arg);
    printf("get new sockfd:%d",newsockfd);
}