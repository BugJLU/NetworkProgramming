//
// Created by active on 18-4-16.
//
#include <iostream>
#include <cstring>
#include "File.h"

using namespace std;

int main(){

    int length,il,ol;
    cout<<"Input buffer length:"<<endl;
    cin>>length;
    char *buffer = new char[length];

    /*ReadTest*/

    char *ifname = "ReadTest.txt";
    char *ofname = "WriteTest.txt";

    File ifile(ifname,IN);
    File ofile(ofname,OUT);

    do{
        il = ifile.read(buffer, length);  //eof->"/n"
        ol = ofile.write(buffer, il);
        cout << il << ":" << buffer << ":" << ol << endl;
        memset(buffer, 0, sizeof(buffer));
    }while(il == length);

    ifile.close();
    ofile.close();

    /*WriteTest*/

//    char *ofname = "WriteTest.txt";
//    File ofile(ofname,'o');
//    ofile.write(buffer,l);

    delete[] buffer;
    return 0;
}
