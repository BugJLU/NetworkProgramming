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
#include <cstring>
#include "../../FileOperation/File.h"

using namespace std;


pthread_mutex_t commandMutex;
pthread_mutex_t dataMutex;
pthread_mutex_t mapMutex;

MultiSocket commandMulti;
MultiSocket dataMulti;
map<Socket*, File*> sockMap;
bool flag = true;

struct ftpArg
{
    pthread_mutex_t& commandMutex;
    pthread_mutex_t& dataMutex;
    pthread_mutex_t& mapMutex;

    MultiSocket& commandMulti;
    MultiSocket& dataMulti;
    map<Socket, File>& sockMap;
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
    pthread_t commandThreadId;
    pthread_t dataThreadId;
    pthread_create(&commandThreadId, NULL, commandThread, &globalArg);
    pthread_create(&dataThreadId, NULL, dataThread, &globalArg);
    while (flag)
    {
        Socket client = server.accept();
        commandMulti.addSocket(client);
    }
    return 0;
}

void* commandThread(void* arg)
{
    pthread_detach(pthread_self());
    auto farg = (ftpArg*)arg;
    while (1) {

        pthread_mutex_lock(&farg->dataMutex);

        pthread_mutex_unlock(&farg->dataMutex);
    }
}

void* dataThread(void* arg)
{
    pthread_detach(pthread_self());
    ftpArg* farg = (ftpArg*)arg;
    vector<Socket> in = vector();
    vector<Socket> out = vector();
    char request[300];
    char response[5];
    while(flag)
    {
        pthread_mutex_lock(&farg->commandMutex);
        farg->commandMulti.listenAll(in, out);
        for(int i = 0; i < in.size(); i++)
        {
            in[i].recv(request,300);
            if(request[0] == 0)
            {
                char Id = request[1];
                unsigned  int port = request[2] << 8 + request[3];
                unsigned  int length = request[4];
                char filename[length];
                memcpy(filename, request + 5, length);
                File* file = new File();
                response[0] = Id;
                if(file->open(filename, 'r') != -1)
                {
                    for(int j = 4; j > 0; j--)
                    {
                        response[j] = length - length >> 8 << 8;
                        length = length >> 8;
                    }
                    in[i].send(response,5);
                    InetAddr clientAddress = in[i].getAddress();
                    clientAddress.setPort(port);
                    Socket* client = new Socket();
                    if( client->connect(clientAddress) != -1 )
                    {
                        unsigned int fileLength = file->getFilelength();
                        char serverrequest[3+length+fileLength];
                        serverrequest[1] = 0;
                        serverrequest[2] = Id;
                        serverrequest[3] = length;
                        for(int j = 0; j < length; j++)
                        {
                            serverrequest[j+3] = filename[j];
                        }
                        for(int j = 6+length; j > 2+length; j--)
                        {
                            serverrequest[j] = fileLength - fileLength >> 8 << 8;
                            fileLength = fileLength >> 8;
                        }
                        client->send(serverrequest, 3+length+fileLength);
                    }
                    pthread_mutex_lock(&farg->mapMutex);
                    farg->sockMap.insert(pair<Socket*,File*>(client, file));
                    pthread_mutex_unlock(&farg->mapMutex);
                    pthread_mutex_lock(&farg->dataMutex);
                    farg->dataMulti.addSocket(*client);
                    pthread_mutex_unlock(&farg->dataMutex);

                }else
                {
                    for(int j = 1; j < 4; j++)
                    {
                        response[j] = 0;
                    }
                    response[4] = -1;
                    in[i].send(response,5);
                }

            }
        }
        pthread_mutex_unlock(&farg->commandMutex);
    }
}