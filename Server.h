/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */


#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CLI.h"
#include "commands.h"

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file



// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        CLI* cli = new CLI(new SocketDio(clientID));
        cli->start();
        delete cli;
    }
};


// implement on Server.cpp
class Server {
    thread* t; // the thread to run the start() method in
    int sock_fd;
    sockaddr_in server_sockaddr;
    sockaddr_in client_sockaddr;
    bool running;

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */
