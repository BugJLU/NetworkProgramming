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


pthread_mutex_t commandMutex;
pthread_mutex_t dataMutex;
pthread_mutex_t mapMutex;

MultiSocket commandMulti;
MultiSocket dataMulti;
map<Socket, File> sockMap;

struct ftpArg
{
    pthread_mutex_t commandMutex;
    pthread_mutex_t dataMutex;
    pthread_mutex_t mapMutex;

    MultiSocket commandMulti;
    MultiSocket dataMulti;
    map<Socket, File> sockMap;
};

void* commandThread(void*);
void* dataThread(void*);


int main()
{
    return 0;
}

void* commandThread(void* arg)
{
    auto farg = (ftpArg*)arg;
    while (1) {

        pthread_mutex_lock(&farg->dataMutex);

        pthread_mutex_unlock(&farg->dataMutex);
    }
}

void* dataThread(void* arg)
{

}