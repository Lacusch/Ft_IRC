#include <string>

#include "../incl/Response.hpp"
#include "../incl/Server.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

std::string Server::messageCreator(int fd, std::string status, std::string trailing) {
    std::string res = "";
    res += ":" + this->getName() + " " + status + " ";
    std::string nickName = _clients[fd]->getNickName();
    if (nickName.length() > 0)
        res += _clients[fd]->getNickName() + " ";
    else
        res += "* ";
    res += ":" + trailing + "\r\n";
    return (res);
}

int Server::sendMessage(int fd, Res res) {
    Status status;
    status = Response::create_response(res);
    std::string msg = this->messageCreator(fd, std::to_string(status.code), status.msg);
    int res_status = send(fd, msg.c_str(), msg.size(), 0);
    if (res_status == -1) Utils::print(R, "Error sending message");
    return (1);
}

int Server::handlePass(int fd, Request req) {
    if (req.getParams().size() < 1) return (sendMessage(fd, NOT_ENOUGH_PARAMS));
    // if (_clients[fd]->isAuthenticated()) {
    //         std::string res = ":" + this->getName() + " 462 * :User already registered \r\n";
    //         return (sendMessage(fd, res));
    // }
    // if (req.getParams().size() == 1 && req.getParams()[0] != this->getPassword()) {
    //         std::string res =
    //             ":" + this->getName() + " 464 * :Incorrect password. Please try again. \r\n";
    //         return (sendMessage(fd, res));
    // }
    if (req.getParams().size() == 1 && req.getParams()[0] == this->getPassword()) {
        _clients[fd]->setAuthentication(true);
    }
    return (1);
}