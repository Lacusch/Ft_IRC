#pragma once
#include <iostream>

class Server {
   private:
    int _iport;
    int _fd;
    std::string _port;
    std::string _password;

   public:
    Server(std::string port, std::string password);
    Server(const Server &rhs);
    Server &operator=(const Server &rhs);
    ~Server();

    int valid_args();
    int start_server();
};