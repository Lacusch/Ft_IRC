#pragma once

#include "Client.hpp"
#include "Shared.hpp"

class Server {
   private:
    int _iport;
    int _fd;
    std::string _name;
    std::string _port;
    std::string _password;
    std::vector<pollfd> _sockets;
    std::map<int, Client *> _clients;

   public:
    Server(std::string name, std::string port, std::string password);
    Server(const Server &rhs);
    Server &operator=(const Server &rhs);
    ~Server();

    std::string getName() const;
    std::string getPassword() const;

    int valid_args();
    int start_server();
    int run();
    int newClient();
    int clientMessage(int i);
};