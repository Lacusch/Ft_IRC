#pragma once
#include "Shared.hpp"

// -----------------------
// Request class creates a request to the server
// -----------------------
class Request {
   private:
    int _fd;
    int _receiver_fd;
    std::string _prefix;
    std::string _command;
    std::vector<std::string> _params;
    std::string _trailing;
    std::vector<std::pair<std::string, std::string> > _join_params;

   public:
    Request();
    ~Request();

    // -----------------------
    // Setters
    // -----------------------
    void setFd(int fd);
    void setReceiverFd(int fd);
    void setPrefix(std::string arg);
    void setCommand(std::string arg);
    void setTrailing(std::string arg);
    void setParams(std::string arg);
    void setJoinParams(std::vector<std::string> channels, std::vector<std::string> keys);

    // -----------------------
    // Getters
    // -----------------------
    int getFd() const;
    int getReceiverFd() const;
    std::string getPrefix() const;
    std::string getCommand() const;
    std::string getTrailing() const;
    std::vector<std::string> getParams() const;
    std::vector<std::pair<std::string, std::string> > getJoinParams() const;
};