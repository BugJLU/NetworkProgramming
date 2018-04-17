//
// Created by active on 18-4-16.
//

#ifndef NETWORKPROGRAMMING_FILE_H
#define NETWORKPROGRAMMING_FILE_H


#include <iostream>
#include <fstream>
#include <cstdlib>

#define IN 'i'
#define OUT 'o'

using namespace std;

class File {
private:
    const char *filename;
    char mode;
    bool flag;
    ofstream ofs;
    ifstream ifs;
public:
    File();
    File(const char *fname,char m);
    int open(const char *fname,char m);
    int close();
    int read(char *buffer,int length);
    int write(const char *buffer,int length);
};


#endif //NETWORKPROGRAMMING_FILE_H
