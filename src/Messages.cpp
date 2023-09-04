#include <sys/socket.h>

#include <cctype>
#include <string>

#include "../incl/Response.hpp"
#include "../incl/Server.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

std::string Server::messageCreator(int fd, Status status) {
    std::string res = "";
    res += ":" + this->getName() + " " + status.code + " ";
    std::string nickName = _clients[fd]->getNickName();
    if (nickName.length() > 0)
        res += _clients[fd]->getNickName() + " ";
    else
        res += "* ";
    if (status.extra.length()) res += status.extra + " ";
    res += ":" + status.msg + "\r\n";
    Utils::print(P, res);
    return (res);
}

int Server::sendMessage(int fd, Res res, Request req) {
    Status status;
    status = Response::create_response(res, req);
    std::string msg = this->messageCreator(fd, status);
    int res_status = send(fd, msg.c_str(), msg.size(), 0);
    if (res_status == -1) Utils::print(R, "Error sending message");
    return (1);
}

int Server::handlePassword(int fd, Request req) {
    if (req.getParams().size() < 1) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    if (req.getParams().size() > 1) return (sendMessage(fd, ENOUGH_PARAMS, req));
    if (_clients[fd]->isAuthenticated()) return (sendMessage(fd, USER_ALREADY_REGISTERED, req));
    if (req.getParams().size() == 1 && req.getParams()[0] != this->getPassword())
        return (sendMessage(fd, INCORRERCT_PASSWORD, req));
    if (req.getParams().size() == 1 && req.getParams()[0] == this->getPassword())
        _clients[fd]->setAuthentication(true);
    return (0);
}

int Server::handleNickName(int fd, Request req) {
    if (req.getParams().size() < 1) return (sendMessage(fd, NO_NICKNAME_GIVEN, req));
    if (req.getParams().size() > 1) return (sendMessage(fd, ENOUGH_PARAMS, req));
    std::string nickname = req.getParams()[0];
    for (size_t i = 0; i < nickname.size(); i++) {
        if (!(std::isalnum(nickname[i]) || nickname[i] == '-' || nickname[i] == '_' ||
              nickname[i] == '|'))
            return (sendMessage(fd, ERRONEOUS_NICKNAME, req));
    }
    if (this->nickNameInUse(nickname)) return (sendMessage(fd, NICKNAME_IN_USE, req));
    _clients[fd]->setNickName(nickname);
    return (0);
}