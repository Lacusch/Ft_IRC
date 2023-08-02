
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) return (1);
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);  // Set your desired port here
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket." << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening on socket." << std::endl;
        close(server_socket);
        return 1;
    }

    std::vector<pollfd> clientSockets;
    pollfd serverPollFd;
    serverPollFd.fd = server_socket;
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
                accept(server_socket, (struct sockaddr*)&clientAddress, &clientAddressLength);
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
                std::cout << "data" << std::endl;
                // Handle incoming data from the client at index i
                // ...
            }
        }
    }

    // Close all client sockets
    for (size_t i = 1; i < clientSockets.size(); ++i) {
        close(clientSockets[i].fd);
    }
    close(server_socket);
    return 0;
};
