//
// Created by leon on 18-4-18.
//

#include <iostream>
#include "../../Multiplexing/MultiSocket.h"
#include "../../Socket/InetAddr.h"
#include "../../Socket/ServerSocket.h"
#include "../../Socket/Socket.h"
#include "../../Multiplexing/MultiSocket.h"
#include <map>
#include "../../FileOperation/File.h"

using namespace std;

#define SLICE_LEN 1024

pthread_mutex_t commandMutex;
pthread_mutex_t dataMutex;
pthread_mutex_t mapMutex;

MultiSocket commandMulti;
MultiSocket dataMulti;
map<Socket*, File*> sockMap;

struct ftpArg
{
    pthread_mutex_t& commandMutex;
    pthread_mutex_t& dataMutex;
    pthread_mutex_t& mapMutex;

    MultiSocket& commandMulti;
    MultiSocket& dataMulti;
    map<Socket*, File*>& sockMap;
};

void* commandThread(void*);
void* dataThread(void*);


int main()
{
    ServerSocket server = ServerSocket(7777);
    pthread_mutex_init(&commandMutex,NULL);
    pthread_mutex_init(&dataMutex,NULL);
    pthread_mutex_init(&mapMutex,NULL);
    commandMulti = MultiSocket();
    dataMulti = MultiSocket();
    sockMap = map();
    ftpArg globalArg = ftpArg();
    globalArg.commandMutex = commandMutex;
    globalArg.dataMutex = dataMutex;
    globalArg.mapMutex = mapMutex;
    globalArg.commandMulti = commandMulti;
    globalArg.dataMulti = dataMulti;
    globalArg.sockMap = sockMap;
    return 0;
}

void* commandThread(void* arg)
{
    pthread_detach(pthread_self());
    auto farg = (ftpArg*)arg;
    vector<Socket> in, out;
    char* buf = new char[SLICE_LEN];
    while (1) {
        pthread_mutex_lock(&farg->dataMutex);
        farg->dataMulti.listenAll(in, out);
        pthread_mutex_unlock(&farg->dataMutex);
        for (int i = 0; i < out.size(); ++i) {
            Socket& sock = in[i];
            auto fileToSend = farg->sockMap[&sock];
            auto len = fileToSend->read(buf, SLICE_LEN);
            sock.send(buf, len);
            if (fileToSend->ieof()) {

                farg->sockMap.erase(&sock);
                sock.close();
                delete &sock;
                fileToSend->close();
                delete fileToSend;
                // TODO： potential risk!!
                in.erase(in.begin()+i);
                --i;
            }
        }
    }
    delete[] buf;
}

void* dataThread(void* arg)
{

}