//
// Created by active on 18-4-16.
//

#ifndef NETWORKPROGRAMMING_FILE_H
#define NETWORKPROGRAMMING_FILE_H


#include <iostream>
#include <fstream>
#include <cstdlib>

#define FILE_IN 0b0001
#define FILE_OUT 0b0010

//using namespace std;

class File {
private:
//    const char *filename;
//    char mode;
//    bool flag;
    int filelength;
    std::ofstream ofs;
    std::ifstream ifs;
public:
    File();
    File(const char *fname,char m);
    int getFilelength();
    bool oeof() {
        return ofs.eof();
    };
    bool ieof() {
        return ifs.eof();
    }
    int open(const char *fname,char m);
    int close();
    int read(char *buffer,int length);
    int write(const char *buffer,int length);
};


#endif //NETWORKPROGRAMMING_FILE_H
