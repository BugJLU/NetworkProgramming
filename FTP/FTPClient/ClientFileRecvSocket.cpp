//
// Created by active on 18-4-18.
//

#include "ClientFileRecvSocket.h"

ClientFileRecvSocket::ClientFileRecvSocket() {}

ClientFileRecvSocket::ClientFileRecvSocket(Socket s) {
    this->sSocket = s;
    execute();
}

int ClientFileRecvSocket::execute() {
    setFileInfo();
    char buffer[256];
    //TODO
    sSocket.recv(buffer,fInfo.fileLen);
    //TODO fileoperation
    return 0;
}

int ClientFileRecvSocket::setFileInfo() {

    char buffer[3];
    sSocket.recv(buffer,3);
    char m = buffer[0];
    char i = buffer[1];
    char fnl = buffer[2];
    char fname[fnl];
    sSocket.recv(fname,fnl);
    char flen[4];
    sSocket.recv(flen,4);

    fInfo.mode = m;
    fInfo.ID = i;
    fInfo.fileNameLen = fnl;
    fInfo.fileName = fname;
    //TODO
    fInfo.fileLen = 0;

    return 0;
}

FileInfo ClientFileRecvSocket::getFileInfo() {
    return fInfo;
}

int ClientFileRecvSocket::setFileStatus() {
    fStatus.ID = fInfo.ID;
    //TODO
    fStatus.status = false;
    return 0;
}

FileStatus ClientFileRecvSocket::getFileStatus() {
    return fStatus;
}

