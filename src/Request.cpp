#include "../incl/Request.hpp"
#include "../incl/Utils.hpp"

Request::Request() {}

Request::~Request() {}

int Request::getFd() const { return _fd; }

int Request::getReceiverFd() const { return _receiver_fd; }

std::string Request::getPrefix() const { return _prefix; }

std::string Request::getTrailing() const { return _trailing; }

std::string Request::getCommand() const { return _command; }

std::vector<std::string> Request::getParams() const { return _params; }

std::vector<std::pair<std::string, std::string> > Request::getJoinParams() const {
    return _join_params;
}

void Request::setFd(int fd) { this->_fd = fd; }

void Request::setReceiverFd(int fd) { this->_receiver_fd = fd; }

void Request::setPrefix(std::string arg) { this->_prefix = arg; }

void Request::setCommand(std::string arg) { this->_command = arg; }

void Request::setTrailing(std::string arg) { this->_trailing = arg; }

void Request::setParams(std::string arg) { this->_params.push_back(arg); }

void Request::setJoinParams(std::vector<std::string> channels, std::vector<std::string> keys) {

    for (size_t i = 0; i < channels.size(); ++i) {
        std::string first = Utils::removeSpace(channels[i]);
        std::string second = (i < keys.size()) ? Utils::removeSpace(keys[i]) : "";
        _join_params.push_back(std::make_pair(first, second));
    }

}