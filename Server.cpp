
#include "Server.h"

Server::Server(int port)throw (const char*) {
    running = true;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        throw "Failed to create socket";
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = INADDR_ANY;
    server_sockaddr.sin_port = htons(port);

    if (bind(sock_fd, (struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)) < 0)
        throw "Failed to bind server.";

    if(listen(sock_fd, 3)< 0)
        throw "Failed to listen.";

}

void Server::start(ClientHandler& ch)throw(const char*){
        t = new thread([&ch, this]() {
            socklen_t size = sizeof(client_sockaddr);
            while (running) {
                alarm(1);
                int client = accept(sock_fd, (struct sockaddr *) &client_sockaddr, &size);
                alarm(0);
                if (client < 0)
                    throw "Failed to accept client.";
                ch.handle(client);
                close(client);
            }
        });
}

void Server::stop(){
    running = false;
    t->join(); // do not delete this!
    close(sock_fd);
}

Server::~Server() {
    delete t;
}