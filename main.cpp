#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#define SERVER_ADDR "127.0.0.1"  // localhost
#define SERVER_PORT 6667

int main() {
    // ---------------------
    // Setup for Server
    // ---------------------

    // Creating server socket
    // AF_INET = IPv4
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        std::cout << "error creating socket" << std::endl;
        return (1);
    }
    std::cout << "Server socketfd: " << server_socket_fd << std::endl;

    // ---------------------
    // Setting up server address in port 6667
    // ---------------------
    sockaddr_in server_adress;
    bzero(&server_adress, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = inet_addr(SERVER_ADDR);  // only accept connections from this IP
    // server_adress.sin_addr.s_addr = INADDR_ANY;           // accept connections from any address
    server_adress.sin_port = htons(SERVER_PORT);

    // ---------------------
    // Binding server fd to server address
    // ---------------------
    if (bind(server_socket_fd, (struct sockaddr *)&server_adress, sizeof(server_adress)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(server_socket_fd);
        return (1);
    }
    // ---------------------
    // Listen to check if server socket works
    // ---------------------
    if (listen(server_socket_fd, 5) == -1) {
        std::cerr << "Error listening on socket." << std::endl;
        close(server_socket_fd);
        return (1);
    }
    // ---------------------
    // Displaying info about the server
    // ---------------------
    // Displaying port number 6667 from server class
    std::cout << "Sever Port: " << htons(server_adress.sin_port) << std::endl;

    sockaddr_in my_addr;
    unsigned int my_port;
    char my_ip[16];

    bzero(&my_addr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
    getsockname(server_socket_fd, (struct sockaddr *)&my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, my_ip, sizeof(my_ip));
    my_port = ntohs(my_addr.sin_port);

    std::cout << "Server IP: " << my_ip << std::endl;
    std::cout << "Server Port: " << my_port << std::endl;

    // ---------------------
    // Setup for clients
    // ---------------------

    std::vector<pollfd> clientSockets;
    pollfd serverPollFd;
    serverPollFd.fd = server_socket_fd;
    serverPollFd.events = POLLIN;
    clientSockets.push_back(serverPollFd);

    while (true) {
        int activity = poll(clientSockets.data(), clientSockets.size(), -1);
        if (activity == -1) {
            std::cerr << "Error in poll." << std::endl;
            break;
        }

        // Check if there's an incoming connection request on the server socket
        if (clientSockets[0].revents & POLLIN) {
            sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);
            int newClientSocket =
                accept(server_socket_fd, (struct sockaddr *)&clientAddress, &clientAddressLength);
            if (newClientSocket == -1) {
                std::cerr << "Error accepting connection." << std::endl;
            } else {
                std::cout << "New client connected." << std::endl;
                pollfd newClientPollFd;
                newClientPollFd.fd = newClientSocket;
                newClientPollFd.events = POLLIN;
                clientSockets.push_back(newClientPollFd);
            }
        }

        // Check for data on the client sockets
        for (size_t i = 1; i < clientSockets.size(); ++i) {
            if (clientSockets[i].revents & POLLIN) {
                // std::cout << clientSockets[i].fd;
                // Handle incoming data from the client at index i
                // ...
            }
        }
    }

    // Close all client sockets
    for (size_t i = 1; i < clientSockets.size(); ++i) {
        close(clientSockets[i].fd);
    }
    close(server_socket_fd);
    return 0;
};