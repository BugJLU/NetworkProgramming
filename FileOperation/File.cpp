//
// Created by active on 18-4-16.
//

#include "File.h"


File::File() {
    this->filelength = 0;
}

File::File(const char *fname,char m) {
//    this->filename = fname;
//    this->mode = m;
    this->filelength = 0;
    this->open(fname, m);
}

int File::getFilelength() {
    return filelength;
}

int File::open(const char *fname,char m) {
    if(m==FILE_IN) {
        ifs.open(fname,std::ios::in|std::ios::binary);
        ifs.seekg(0,std::ios::end);
        filelength = ifs.tellg();
        ifs.seekg(0,std::ios::beg);
    }else if(m==FILE_OUT){
        ofs.open(fname,std::ios::out|std::ios::trunc|std::ios::binary);
    }else{
//        cout<<"Unknown open mode"<<endl;
        throw "Unknown open mode";
    }
    if(ifs.is_open()||ofs.is_open()){
//        flag = true;
        return 0;
    }else{
//        cout<<"File Open Error!"<<endl;
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
    if(ifs.is_open()|ofs.is_open()){
//        cout<<"Close Error!"<<endl;
//        throw "close error";
        return -1;
    }
    filelength = 0;
//    flag = false;
    return 0;
}

int File::read(char *buffer, int length) {
    int l,cur,end;
    if(ifs.is_open()){
        cur = ifs.tellg();
        ifs.seekg(0,std::ios::end);
        end = ifs.tellg();
        l = end -cur;
        ifs.seekg(cur);
        ifs.read(buffer,length);
    }else{
//        cout<<"No open infile!"<<endl;
//        throw "no input file";
        return -1;
    }
    if(l>=length){
        l = length;
    }
    return l;
}

int File::write(const char *buffer, int length) {
    int l,cur_start,cur_end;
    if(ofs.is_open()){
        cur_start = ofs.tellp();
        ofs.write(buffer,length);
        cur_end = ofs.tellp();
        l = cur_end - cur_start;
        filelength = cur_end;
    }else{
//        cout<<"No open outfile!"<<endl;
//        throw "no output file";
        return -1;
    }
    return l;
}
