#include "server.hh"

Server::Server(unsigned short port) : port(port) {
    // Create socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        std::cerr << "Failed to create socket\n";
        exit(EXIT_FAILURE);
    }

    // Initialize address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Bind to all interfaces
    address.sin_port = htons(port);        // Convert port to network byte order
}

Server::~Server() {
    std::cout<< "Server closing socket."<<std::endl;
    close(fd); 
}

int Server::acceptConnection_() {
    int addrlen = sizeof(address);
    int newSocket = accept(fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (newSocket < 0) {
        std::cerr << "Failed to accept connection\n";
        return -1;
    }
    std::cout << "Connection accepted\n";

    const int bufferSize = 1024;  // Define buffer size
    char buffer[bufferSize];

    // Keep reading and echoing back until the client disconnects
    while (true) {
        int bytesRead = recv(newSocket, buffer, bufferSize, 0);
        if (bytesRead <= 0) {
            std::cout << "Client disconnected or error reading data\n";
            break;
        }
        printf(buffer, bytesRead);
        // Echo data back to client
        // send(newSocket, buffer, bytesRead, 0);
    }

    return newSocket;
}

bool Server::bindAndListen(int backlog) {
    // Bind the socket
    if (bind(fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Binding failed\n";
        return false;
    }

    // Listen for incoming connections
    if (listen(fd, backlog) < 0) {
        std::cerr << "Listening failed\n";
        return false;
    }

    std::cout << "Server listening on port " << port << "\n";

    while(true) {
        acceptConnection_();
    }
    return true;
}

