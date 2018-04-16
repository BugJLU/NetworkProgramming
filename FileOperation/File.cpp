//
// Created by active on 18-4-16.
//

#include "File.h"

File::File() {}

File::File(string fname,char m) {
    this->filename = fname;
    this->mode = m;
    open(filename,mode);
}

int File::open(string fname,char m) {
    if(m=='i') {
        ifs.open(fname,ios::in|ios::binary);
    }else if(m=='o'){
        ofs.open(fname,ios::out|ios::trunc|ios::binary);
    }else{
        cout<<"Unknown open mode"<<endl;
    }
    if(ifs.is_open()||ofs.is_open()){
        return 0;
    }else{
        cout<<"File Open Error!"<<endl;
        return -1;
    }
}

int File::close() {
    if(ifs.is_open()){
        ifs.close();
    }
    if(ofs.is_open()){
        ofs.close();
    }
    if(!ifs.is_open()&&!ofs.is_open()){
        cout<<"Close Error!"<<endl;
        return -1;
    }
    return 0;
}

int File::read(char *buffer, int length) {
    //TODO len of read
    if(ifs.is_open()){
        ifs.read(buffer,length);
    }else{
        cout<<"No open infile!"<<endl;
        return -1;
    }
    return 0;
}

int File::write(const char *buffer, int length) {
    if(ofs.is_open()){
        ofs.write(buffer,length);
    }else{
        cout<<"No open outfile!"<<endl;
        return -1;
    }
    return 0;
}
