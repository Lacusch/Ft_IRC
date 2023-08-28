#pragma once
#include <poll.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>

#include "Client.hpp"

class Server {
   private:
    int _iport;
    int _fd;
    std::string _port;
    std::string _password;
    std::vector<pollfd> _sockets;
    std::vector<Client> _clients;

   public:
    Server(std::string port, std::string password);
    Server(const Server &rhs);
    Server &operator=(const Server &rhs);
    ~Server();

    int valid_args();
    int start_server();
    int run();
    int newClient();
    int clientMessage(int i);
};