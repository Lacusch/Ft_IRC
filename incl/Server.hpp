#pragma once
#include <iostream>

class Server {
   private:
    int _port;
    std::string _password;

   public:
    Server();
    Server(const Server &rhs);
    Server &operator=(const Server &rhs);
    ~Server();

    bool valid_args(std::string port, std::string password);
};