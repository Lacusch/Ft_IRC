#include "../incl/Request.hpp"

Request::Request() {}

Request::~Request() {}

int Request::getFd() const { return _fd; }

int Request::getReceiverFd() const { return _receiver_fd; }

std::string Request::getPrefix() const { return _prefix; }

std::string Request::getTrailing() const { return _trailing; }

std::string Request::getCommand() const { return _command; }

std::vector<std::string> Request::getParams() const { return _params; }

void Request::setFd(int fd) { this->_fd = fd; }

void Request::setReceiverFd(int fd) { this->_receiver_fd = fd; }

void Request::setPrefix(std::string arg) { this->_prefix = arg; }

void Request::setCommand(std::string arg) { this->_command = arg; }

void Request::setTrailing(std::string arg) { this->_trailing = arg; }

void Request::setParams(std::string arg) { this->_params.push_back(arg); }