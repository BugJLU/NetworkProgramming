//
// Created by active on 18-4-15.
//

#ifndef NETWORKPROGRAMMING_TCPFORK_H
#define NETWORKPROGRAMMING_TCPFORK_H



class TcpFork {
private:
    int port;
public:
    explicit  TcpFork(int port);
    int execute();
};


#endif //NETWORKPROGRAMMING_TCPFORK_H
