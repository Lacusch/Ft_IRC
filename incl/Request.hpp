#pragma once
#include "Shared.hpp"

/*
 * Request class creates a request to the server
 */
class Request {
   private:
    int _fd;
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _params;
    std::string _trailing;

   public:
    Request();
    ~Request();

    // setters
    void setFd(int fd);
    void setPrefix(std::string arg);
    void setCommand(std::string arg);
    void setTrailing(std::string arg);
    void setParams(std::string arg);

    // Getters
    int getFd() const;
    std::string getPrefix() const;
    std::string getCommand() const;
    std::string getTrailing() const;
    std::vector<std::string> getParams() const;
};