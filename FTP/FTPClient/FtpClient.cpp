//
// Created by blacksun on 18-4-18.
//
#include"FtpClient.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

#include <time.h>
#include <stdlib.h>

#define MAXACC 10


FtpClient::FtpClient(char*server) {
    this->server = gethostbyname(server);
    srand((int)time(0));
    //portno = (unsigned short)(rand() % 65536);
    portno = 53687;
}

void FtpClient::start() {

    pthread_t cmd_tid, data_tid;
    printf("client listen on port:%d\n",portno);
    if(pthread_create(&cmd_tid,NULL,processCmd,(void*)server)!=0){
        printf("pthread cmd create error");
        exit(-1);
    }
    if(pthread_create(&data_tid,NULL,processData,NULL)!=0){
        printf("pthread data create error");
        exit(-1);
    }
    pthread_join(data_tid,NULL);
    pthread_join(cmd_tid,NULL);


}

void* FtpClient::processCmd(void *arg) {
    int sockfd, n;
    char send_buffer[261];
    char recv_buffer[6];
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
    while(running){
        printf("Please enter the command: ");
        scanf("%s",cmd);
        if(strcmp(cmd,"get") == 0){
            scanf("%s",filename);
            send_buffer[0] = 0;
        }else if(strcmp(cmd,"quit") == 0){
            running = false;
            break;
        }else{
            running = false;
            printf("unvalid command");
            break;
        }

        if((filelen = strlen(filename))>0){
            send_buffer[1] = cmdcount;
            send_buffer[2] = portno>>8;
            send_buffer[3] = portno & 0x00ff;
            send_buffer[4] = filelen;
            for(int i = 1;i<=filelen;i++){
                send_buffer[4+i] = filename[i-1];
            }
            send_buffer[5+filelen] = '\0';
            if((n = write(sockfd, send_buffer,5+filelen))<0){
                perror("cmd:Error writing to socket");
                running = false;
                break;
            }
            if((n = read(sockfd, recv_buffer,sizeof(recv_buffer)))!=5){
                printf("get file error\n");
            }else{
                unsigned char id = recv_buffer[0];
                int filelen;
                char* p = (char*)&filelen;
                for(int i = 0;i<4;i++)
                    p[i] = recv_buffer[4-i];
                if(filelen>=0&&id == cmdcount){
                    printf("file exist(filelen:%d)\n",filelen);
                }else{
                    printf("file not exist or error\n");
                }
            }
            cmdcount++;

        }

    }
    close(sockfd);
    return 0;
}

void* FtpClient::processData(void *arg) {
    int sockfd, newsockfd, clicount;
    //unsigned short portno;
    struct sockaddr_in serv_addr, cli_addr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("data:Error opening socket");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("data:Error on binding");
        exit(1);
    }
    listen(sockfd,MAXACC);
    socklen_t clilen = sizeof(cli_addr);
    while(running){
        if((newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen))<0){
            perror("data:Error on accept");
            break;
        }
        printf("accept a connect(addr:%s, port:%d)\n",
               inet_ntoa(cli_addr.sin_addr),cli_addr.sin_port);
        pthread_t trans_tid;
        Info info;
        Socket accSocket;
        accSocket.setFd(newsockfd);
        info.serverSocket = accSocket;
        info.fsQueue = FtpClient::fsQueue;
        //info.qMutex = FtpClient::qMutex;
        if(pthread_create(&trans_tid,NULL,processTrans,(void*)&info)!=0){
            printf("pthread trans create error");
            exit(-1);
        }
    }
    close(sockfd);
    return 0;
}

void* FtpClient::processTrans(void *arg) {
    Info* info = (Info*)(arg);
    ClientFileRecvSocket cfrs(info);

}
/*bool FtpClient::init() {
    pthread_mutex_init(FtpClient::qMutex,NULL);

}*/
//bool FtpClient::__init = FtpClient::init();
//pthread_mutex_t FtpClient::qMutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<FileStatus>* FtpClient::fsQueue = new std::vector<FileStatus>;
bool FtpClient::running = true;
unsigned short FtpClient::portno = 0;


