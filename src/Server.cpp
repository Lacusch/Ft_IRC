
#include "../incl/Server.hpp"

#include "../incl/Channel.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

Server::Server(std::string name, std::string version, std::string port, std::string password)
    : _name(name),
      _version(version),
      _creation_date(std::time(NULL)),
      _port(port),
      _password(password){};

Server::~Server() { clean(); };

void Server::clean(void) {
    // Close all sockets
    for (std::vector<pollfd>::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        close(it->fd);
    }
    // Delete all Client objects in the map
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        delete it->second;
    }
    // Delete all Channel objects in the map
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end();
         ++it) {
        delete it->second;
    }

    _sockets.clear();
    _clients.clear();
    _channels.clear();

    _sockets = std::vector<pollfd>();
    _channels = std::map<std::string, Channel*>();
    _clients = std::map<int, Client*>();
}

std::string Server::getName() const { return (this->_name); }

std::string Server::getVersion() const { return (this->_version); }

std::string Server::getPort() const { return (this->_port); }

std::string Server::getPassword() const { return (this->_password); }

std::map<int, Client*> Server::getClientsList() const { return (this->_clients); }

std::string Server::getCreationDate() const {
    std::string datetime = std::ctime(&_creation_date);
    return (datetime);
}

// Returns 1 on error
int Server::valid_args() {
    int port_int = 0;
    if (_port.empty() || _password.empty()) return (Utils::print_error(EMPTY_ARGS));
    for (const char* ptr = _port.c_str(); *ptr != '\0'; ++ptr) {
        if (!std::isdigit(*ptr)) return (Utils::print_error(NUMERIC_PORT));
        port_int = port_int * 10 + (*ptr - '0');
    }
    if (port_int > 6669 || port_int < 6665) return (Utils::print_error(PORT_RANGE));
    _iport = port_int;
    if (_password.length() > 50) return (Utils::print_error(PASS_TOO_LONG));
    return (0);
};

// Returns 1 on error
int Server::start_server() {
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) return (Utils::print_error(CREATING_SOCKET));

    int reuse = 1;
    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        close(server_socket_fd);
        return (Utils::print_error(CONFIGURING_SOCKET));
    }

    _fd = server_socket_fd;
    sockaddr_in server_adress;
    bzero(&server_adress, sizeof(server_adress));
    server_adress.sin_family = AF_INET;
    server_adress.sin_port = htons(_iport);
    server_adress.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket_fd, (struct sockaddr*)&server_adress, sizeof(server_adress)) == -1) {
        close(server_socket_fd);
        return (Utils::print_error(BINDING_SOCKET));
    }

    if (listen(server_socket_fd, server_socket_fd + 1) == -1) {
        close(server_socket_fd);
        return (Utils::print_error(LISTENING_SOCKET));
    };

    pollfd serverPollFd;
    serverPollFd.fd = server_socket_fd;
    serverPollFd.events = POLLIN;
    _sockets.push_back(serverPollFd);
    return (0);
}

void Server::hello() const {
    Utils::print(CGR, "--------------------");
    Utils::print(CGR, "-- " + this->getName());
    Utils::print(CGR, "-- Welcome to IRC");
    Utils::print(CGR, "-- port: " + this->getPort());
    Utils::print(CGR, "-- version: " + this->getPort());
    Utils::print(CGR, "-- password: " + this->getPassword());
    Utils::print(CGR, "-- timestamp: " + getCreationDate());
}

int Server::run() {
    hello();
    while (true) {
        int poll_res = poll(_sockets.data(), _sockets.size(), -1);
        if (poll_res == -1) return (1);
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
        return (Utils::print_error(ACCEPTING_CONNECTION));
    } else {
        pollfd newClientPollFd;
        newClientPollFd.fd = newClientSocket;
        newClientPollFd.events = POLLIN;
        _sockets.push_back(newClientPollFd);
        Utils::print(CB, "client connected on FD: " + Utils::to_string(newClientPollFd.fd) + "\n");
        _clients[newClientPollFd.fd] = new Client(newClientPollFd.fd);
    }
    return (0);
}

int Server::deleteUser(int i) {
    int fd = _sockets[i].fd;

    Utils::print(CR, "deleting client FD: " + Utils::to_string(fd));
    close(_sockets[i].fd);

    // std::vector<pollfd>::iterator it_socket = _sockets.begin() + i;
    // _sockets.erase(it_socket);

    handleQuit(fd);

    std::map<int, Client*>::iterator it = _clients.find(_sockets[i].fd);
    if (it != _clients.end()) {
        delete it->second;
        _clients.erase(it);
    };
    return (0);
}

int Server::clientMessage(int i) {
    char buffer[1024];
    ssize_t bytesRead = recv(_sockets[i].fd, buffer, sizeof(buffer), 0);

    int fd = _sockets[i].fd;

    if (bytesRead == 0) {
        deleteUser(i);
    } else if (bytesRead < 0)
        return (Utils::print_error(READING_RECV));
    else {
        std::string msg(buffer, bytesRead);
        _clients[fd]->setMsg(msg);
        if (msg[msg.length() - 1] != '\n') return true;

        Request req = Utils::parse_msg(fd, Utils::irc_trim(_clients[fd]->getMsg()));
        req.setCommand(Utils::to_upper(req.getCommand()));
        _clients[fd]->clearMsg();

        Utils::print_req(req);
        std::string cmd = req.getCommand();

        if (cmd.length() == 0) return (sendMessage(fd, UNKNWON_COMMAND, req));
        if (cmd == "PASS") return (this->handlePassword(fd, req));
        if (!_clients[fd]->isAuthenticated()) return (sendMessage(fd, PROVIDE_PASS, req));
        if (cmd == "NICK") return (this->handleNickName(fd, req));
        if (cmd == "USER") return (this->handleUser(fd, req));
        if (!_clients[fd]->isRegistered() || !_clients[fd]->hasNickname())
            return (sendMessage(fd, REGISTER_FIRST, req));
        else if (cmd == "PING")
            return (this->handlePing(fd, req));
        else if (cmd == "PONG")
            return (true);
        else if (cmd == "NOTICE")
            return (true);
        else if (cmd == "WHO")
            return (this->handleWho(fd, req));
        else if (cmd == "PRIVMSG")
            return (this->handlePrivateMsg(fd, req));
        else if (cmd == "JOIN")
            return (this->handleJoinChannel(fd, req));
        else if (cmd == "MODE")
            return (this->handleMode(fd, req));
        else if (cmd == "KICK")
            return (this->handleKick(fd, req));
        else if (cmd == "INVITE")
            return (this->handleInvite(fd, req));
        else if (cmd == "TOPIC")
            return (this->handleTopic(fd, req));
        else if (cmd == "PART")
            return (this->handlePart(fd, req));
        else if (cmd == "QUIT")
            return (this->handleQuit(fd));
        else if (cmd == "ROLL")
            return (this->handleRollDie(fd, req));
        else
            return (sendMessage(fd, UNKNWON_COMMAND, req));
    }
    return (0);
}
