//
// Created by active on 18-4-18.
//

#include "ClientFileRecvSocket.h"


ClientFileRecvSocket::ClientFileRecvSocket() {

}

ClientFileRecvSocket::ClientFileRecvSocket(Info *ci) {

    /*set nesssary information*/
    //cInfo.qMutex = ci->qMutex;
    cInfo.fsQueue = ci->fsQueue;
    cInfo.serverSocket = ci->serverSocket;
    /*all in one*/
    execute();
}

int ClientFileRecvSocket::execute() {
    /*read file information from serverSocket*/
    setFileInfo();
    /*read file from serverSocket and write*/
    char buffer[BUFFER_LENGTH];
    uint32_t recv_len = 0;
    File ofile;
    int openflag = ofile.open(fInfo.fileName,FILE_OUT);
    //TODO provide error information
    if(openflag == 0) {
        while (recv_len < fInfo.fileLen) {
            uint32_t cur_len = cInfo.serverSocket.recv(buffer, BUFFER_LENGTH);
            ofile.write(buffer,cur_len);
            recv_len = recv_len + cur_len;
        }
        ofile.close();
    }
    /*set status to this*/
    if(recv_len == fInfo.fileLen){
        setFileStatus(0);
    }else if(recv_len<fInfo.fileLen){
        setFileStatus(-1);
    }else if(recv_len>fInfo.fileLen){
        setFileStatus(-2);
    }
    /*set status to queue*/
    setQueue();
    /*close socket*/
    cInfo.serverSocket.close();

    return 0;
}

int ClientFileRecvSocket::setFileInfo() {
    /*read mode,id,fileNameLength*/
    char buffer[3];
    cInfo.serverSocket.recv(buffer,3);
    fInfo.mode = buffer[0];
    fInfo.ID = buffer[1];
    fInfo.fileNameLen = buffer[2];
    /*read file name*/
    char fname[fInfo.fileNameLen];
    cInfo.serverSocket.recv(fname,fInfo.fileNameLen);
    fInfo.fileName = fname;
    /*read file length*/
    char flen[4];
    cInfo.serverSocket.recv(flen,4);
    uint32_t l = 0;

    for (int i = 0; i < 4; ++i) {
        l = l<<8 + flen[i];
    }

    fInfo.fileLen = l;

    return 0;
}



int ClientFileRecvSocket::setFileStatus(int s) {
    fStatus.ID = fInfo.ID;
    fStatus.fileName = fInfo.fileName;
    fStatus.status = s;
    return 0;
}



int ClientFileRecvSocket::setQueue() {
    //pthread_mutex_lock(cInfo.qMutex);
    cInfo.fsQueue->push_back(fStatus);
    //pthread_mutex_unlock(cInfo.qMutex);
    return 0;
}


void* ClientFileRecv(void* arg){
    /*use this*/
    Info *createInfo = (Info*)arg;
    ClientFileRecvSocket cfrs(createInfo);
    //TODO what to return
    return NULL;
}