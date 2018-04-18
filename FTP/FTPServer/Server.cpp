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

#define SLICE_LEN 1024

pthread_mutex_t g_commandMutex;
pthread_mutex_t g_dataMutex;
pthread_mutex_t g_mapMutex;

MultiSocket g_commandMulti;
MultiSocket g_dataMulti;
map<Socket*, File*> g_sockMap;

bool flag = true;

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
    pthread_mutex_init(&g_commandMutex,NULL);
    pthread_mutex_init(&g_dataMutex,NULL);
    pthread_mutex_init(&g_mapMutex,NULL);
    g_commandMulti = MultiSocket();
    g_dataMulti = MultiSocket();
    g_sockMap = map<Socket*, File*>();
    struct ftpArg globalArg{
            g_commandMutex,
            g_dataMutex,
            g_mapMutex,
            g_commandMulti,
            g_dataMulti,
            g_sockMap
    };
    pthread_t commandThreadId;
    pthread_t dataThreadId;
    pthread_create(&commandThreadId, NULL, commandThread, &globalArg);
    pthread_create(&dataThreadId, NULL, dataThread, &globalArg);
    server.listen();
    while (flag)
    {
        Socket client = server.accept();
        g_commandMulti.addSocket(client);
    }
    return 0;
}

void* commandThread(void* arg)
{
    pthread_detach(pthread_self());
    ftpArg* farg = (ftpArg*)arg;
    vector<Socket> in;
    vector<Socket> out;
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
                    InetAddr clientAddress = in[i].getPeerAddr();
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

void* dataThread(void* arg)
{
    pthread_detach(pthread_self());
    auto farg = (ftpArg*)arg;
    vector<Socket> in, out;
    char* buf = new char[SLICE_LEN];
    while (flag) {
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