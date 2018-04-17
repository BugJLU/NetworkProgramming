//
// Created by blacksun on 18-4-15.
//
#include"RawSocket.h"
#include<stdlib.h>
#include<iostream>

#define MAXDATA 100

RawSocket::RawSocket() {

}

void RawSocket::startCat() {
    if((sockfd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) == -1){
        printf("socket error !");
        exit(1);
    }
    catching();
}

void RawSocket::catching() {
    while(true){
        n = recv(sockfd, buf, sizeof(buf), 0);
        if(n<0){
            printf("recv error !\n");
            break;
        }else if(n == 0){
            continue;
        }
        pip = (IPHeader*)buf;
        int iplen = (pip->h_verlen & 0x0f) * 4;
        printf("IP ----- \n"
                          "total length: %d bytes\n"
                          "recv length: %d bytes\n"
                          "header length: %d bytes\n",
               pip->total_len, n, iplen);
        analyseIP();
        if(pip->proto == IPPROTO_TCP){
            ptcp = (TCPHeader*)(buf + iplen);
            analyseTCP();
        }else if(pip->proto == IPPROTO_UDP){
            pudp = (UDPHeader*)(buf + iplen);
            analyseUDP();
        }else if(pip->proto == IPPROTO_ICMP){
            picmp = (ICMPHeader*)(buf + iplen);
            analyseICMP();
        }else{
            printf("\nother protocol !\n\n");
        }
        //printf("\n\n");
    }
    close(sockfd);
    return;
}

void RawSocket::analyseIP() {
    unsigned char * p = (unsigned char*)&pip->sourceIP;
    printf("Source IP :%u.%u.%u.%u\n",p[0],p[1],p[2],p[3]);
    p = (unsigned char *)&pip->destIP;
    printf("Destination IP: %u.%u.%u.%u\n",p[0],p[1],p[2],p[3]);

}

void RawSocket::analyseTCP() {
    int tcplen = (ptcp->th_lenres>>4) * 4;
    printf("TCP ----- (header: %d bytes)\n", tcplen);
    printf("Source port: %u\n",ntohs(ptcp->th_sport));
    printf("Destination port: %u\n",ntohs(ptcp->th_dport));
    printData((unsigned char*)ptcp + tcplen);

}

void RawSocket::analyseUDP() {
    printf("UDP -----\n");
    printf("Source port: %u\n",ntohs(pudp->uh_sport));
    printf("Destination port: %u\n",ntohs(pudp->uh_dport));
    printData((unsigned char*)pudp+sizeof(UDPHeader));
}

void RawSocket::analyseICMP() {
    printf("ICMP ------\n");
    printf("type: %u\n",picmp->icmp_type);
    printf("code: %u\n",picmp->icmp_code);
    printData((unsigned char*)picmp + sizeof(ICMPHeader));
}

void RawSocket::printData(unsigned char *pdata) {
    int len = n - (pdata-buf);
    printf("Data(%d bytes): ",len);
    for(int i = 0; i<MAXDATA&&i<len; i++){
        printf("%c",pdata[i]);
    }
    printf("\n\n");
}