#include "../incl/Server.hpp"

#include <cctype>

#include "../incl/MessageParser.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

Server::Server(std::string name, std::string version, std::string port, std::string password)
    : _name(name), _version(version), _port(port), _password(password){};

Server::~Server() {
    for (size_t i = 0; i < _sockets.size(); i++) close(_sockets[i].fd);
};

std::string Server::getName() const { return (this->_name); }

std::string Server::getPassword() const { return (this->_password); }

int Server::valid_args() {
    int port_int = 0;
    if (_port.empty() || _password.empty()) return (Utils::error(EMPTY_ARGS));
    for (const char* ptr = _port.c_str(); *ptr != '\0'; ++ptr) {
        if (!std::isdigit(*ptr)) return (Utils::error(NUMERIC_PORT));
        port_int = port_int * 10 + (*ptr - '0');
    }
    if (port_int > 6669 || port_int < 6665) return (Utils::error(PORT_RANGE));
    _iport = port_int;
    return (0);
};

int Server::start_server() {
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) return (Utils::error(CREATING_SOCKET));

    int reuse = 1;
    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        close(server_socket_fd);
        return (Utils::error(CONFIGURING_SOCKET));
    }

    _fd = server_socket_fd;
    sockaddr_in server_adress;
    bzero(&server_adress, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(_iport);
    server_adress.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket_fd, (struct sockaddr*)&server_adress, sizeof(server_adress)) == -1) {
        close(server_socket_fd);
        return (Utils::error(BINDING_SOCKET));
    }

    if (listen(server_socket_fd, server_socket_fd + 1) == -1) {
        close(server_socket_fd);
        return (Utils::error(LISTENING_SOCKET));
    };

    pollfd serverPollFd;
    serverPollFd.fd = server_socket_fd;
    serverPollFd.events = POLLIN;
    _sockets.push_back(serverPollFd);

    return (0);
}

int Server::run() {
    while (42) {
        if (poll(_sockets.data(), _sockets.size(), -1) == -1) return (1);
        for (size_t i = 0; i < _sockets.size(); i++) {
            if (_sockets[i].revents & POLLIN) {
                if (_sockets[i].fd == _fd)
                    newClient();
                else
                    clientMessage(i);
            }
        }
    }
    return (0);
}

int Server::newClient() {
    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int newClientSocket = accept(_fd, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (newClientSocket == -1) {
        std::cerr << "Error accepting connection." << std::endl;
    } else {
        std::cout << "New client connected." << std::endl;
        pollfd newClientPollFd;
        newClientPollFd.fd = newClientSocket;
        newClientPollFd.events = POLLIN;
        _sockets.push_back(newClientPollFd);
        Client client(newClientPollFd.fd);
        std::cout << "registering clientfd: " << newClientPollFd.fd << std::endl;
        _clients[newClientPollFd.fd] = &client;
    }
    return (0);
}

int Server::clientMessage(int i) {
    char buffer[1024];
    ssize_t bytesRead = recv(_sockets[i].fd, buffer, sizeof(buffer), 0);

    if (bytesRead == 0) {
        close(_sockets[i].fd);
        Utils::print(B, "Client disconnected");
    } else if (bytesRead < 0)
        Utils::print(R, "Error: recv");
    else {
        int fd = _sockets[i].fd;
        std::string clientMessage(buffer, bytesRead);
        Request req = MessageParser::parseMsg(fd, Utils::parseMsg(clientMessage));

        if (req.getCommand() == "PASS") return (this->handlePassword(fd, req));
        if (!_clients[_sockets[i].fd]->isAuthenticated()) {
            std::string nickname = ":server 464 * :Please provide the server password\r\n";
            return (send(_sockets[i].fd, nickname.c_str(), nickname.size(), 0));
        }
        if (req.getCommand() == "NICK") return (this->handleNickName(fd, req));
        if (req.getCommand() == "USER") return (this->handleUser(fd, req));
    }
    return (true);
}

bool Server::nickNameInUse(std::string nickname) {
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        if (client->getNickName() == nickname) return (true);
    }
    return (false);
}