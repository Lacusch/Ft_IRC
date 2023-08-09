
#include "../incl/Server.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cctype>
#include <iostream>
#include <string>

#include "../incl/Utils.hpp"

Server::Server(std::string port, std::string password) : _port(port), _password(password){};

Server::~Server(){};

int Server::valid_args() {
    int port_int = 0;
    if (_port.empty() || _password.empty()) return (false);
    for (const char* ptr = _port.c_str(); *ptr != '\0'; ++ptr) {
        if (!std::isdigit(*ptr)) return (false);
        port_int = port_int * 10 + (*ptr - '0');
    }
    if (port_int > 6669 || port_int < 6665) {
        Utils::print(Y, "Please use IRC ports: 6665-6669");
        return (false);
    }
    _iport = port_int;
    return (true);
};

int Server::start_server() {
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) return (1);

    _fd = server_socket_fd;
    sockaddr_in server_adress;
    bzero(&server_adress, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = INADDR_ANY;
    server_adress.sin_port = htons(_iport);

    if (bind(server_socket_fd, (struct sockaddr*)&server_adress, sizeof(server_adress)) == -1) {
        close(server_socket_fd);
        return (1);
    }

    if (listen(server_socket_fd, 5) == -1) {
        std::cerr << "Error listening on socket." << std::endl;
        close(server_socket_fd);
        return (1);
    };

    return (0);
}