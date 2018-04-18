//
// Created by blacksun on 18-4-18.
//
#include "FtpClient.h"

int main(){
    //192.168.31.176
    FtpClient fc("192.168.31.32");

    fc.start();

    return 0;
}
