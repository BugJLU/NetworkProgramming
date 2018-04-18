//
// Created by blacksun on 18-4-18.
//
#include "FtpClient.h"

int main(){
    FtpClient fc("192.168.43.143");

    fc.start();

    return 0;
}
