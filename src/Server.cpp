
#include "../incl/Server.hpp"

#include <cctype>
#include <string>

#include "../incl/Utils.hpp"

Server::Server(std::string port, std::string password) : _port(port), _password(password){};

Server::~Server(){};

bool Server::valid_args() {
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