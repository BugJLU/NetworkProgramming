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
    int recv_len = 0;
    File ofile;
    int openflag = ofile.open(fInfo.fileName,FILE_OUT);
    //TODO provide error information
    if(openflag == 0) {
        std::cout<<"file recv start"<<std::endl;
        while (recv_len < fInfo.fileLen) {
            int cur_len = cInfo.serverSocket.recv(buffer, BUFFER_LENGTH);
            ofile.write(buffer,cur_len);
            recv_len = recv_len + cur_len;
        }
        std::cout<<"file recv end"<<std::endl;
        ofile.close();
    }else{
        std::cout<<"file open fail"<<std::endl;
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
    int i =cInfo.serverSocket.recv(buffer,3);
    fInfo.mode = buffer[0];
    fInfo.ID = buffer[1];
    fInfo.fileNameLen = buffer[2];
    /*read file name*/
    char *fname = new char[fInfo.fileNameLen];
    cInfo.serverSocket.recv(fname,fInfo.fileNameLen);
    fInfo.fileName = fname;
    /*read file length*/
    char flen[4];
    int j = cInfo.serverSocket.recv(flen,4);
    int l = 0;

    for (int i = 0; i < 4; ++i) {
        l = int(l<<8) + int(flen[i]<0?(flen[i]+256):flen[i]);
    }

    fInfo.fileLen = l;
    if(i==3&&j==4){
        std::cout<<std::endl<<"set file info success"<<std::endl;
    }
//    printf("%d\n",fInfo.mode);
//    printf("%d\n",fInfo.ID);
//    printf("%d\n",fInfo.fileNameLen);
//    printf("%s\n",fInfo.fileName);
//    printf("%d\n",fInfo.fileLen);
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