#include "../incl/Server.hpp"

Server::Server(){};

Server::~Server(){};

bool Server::valid_args(std::string port, std::string password) {
    if (port.empty() || password.empty()) return (false);
    return (true);
};