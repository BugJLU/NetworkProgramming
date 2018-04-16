//
// Created by active on 18-4-16.
//

#ifndef NETWORKPROGRAMMING_FILE_H
#define NETWORKPROGRAMMING_FILE_H


#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

class File {
private:
    string filename;
    char mode;
    ofstream ofs;
    ifstream ifs;
public:
    File();
    File(string fname,char m);
    int open(string fname,char m);
    int close();
    int read(char *buffer,int length);
    int write(const char *buffer,int length);
};


#endif //NETWORKPROGRAMMING_FILE_H
