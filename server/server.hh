#ifndef HEADER_H
#define HEADER_H

#include <sys/socket.h>
#include <netinet/in.h>  // Defines sockaddr_in
#include <arpa/inet.h>   // Provides inet_pton and htons
#include <unistd.h>      // For close
#include <iostream>


class Server {
private:
    unsigned short port;
    int fd;
    struct sockaddr_in address;
    int acceptConnection_(); 

public:
    Server(unsigned short port);
    ~Server();

    bool bindAndListen(int backlog = 5); // Bind and listen with a backlog
};

#endif