//
// Created by active on 18-4-16.
//

#include "File.h"


File::File() {}

File::File(const char *fname,char m) {
    this->filename = fname;
    this->mode = m;
    this->open(filename,mode);
}

int File::open(const char *fname,char m) {
    if(m=='i') {
        ifs.open(fname,ios::in|ios::binary);
    }else if(m=='o'){
        ofs.open(fname,ios::out|ios::trunc|ios::binary);
    }else{
        cout<<"Unknown open mode"<<endl;
    }
    if(ifs.is_open()||ofs.is_open()){
        flag = true;
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
    if(ifs.is_open()|ofs.is_open()){
        cout<<"Close Error!"<<endl;
        return -1;
    }
    return 0;
}

int File::read(char *buffer, int length) {
    //TODO len of read
    int l,cur,end;
    if(ifs.is_open()){
        cur = ifs.tellg();
        ifs.seekg(0,ios::end);
        end = ifs.tellg();
        l = end -cur;
        ifs.seekg(cur);
        ifs.read(buffer,length);
    }else{
        cout<<"No open infile!"<<endl;
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
    }else{
        cout<<"No open outfile!"<<endl;
        return -1;
    }
    return l;
}
