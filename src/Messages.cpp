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
    status = Response::create_response(res, req, _clients[fd]);
    std::string msg = this->messageCreator(fd, status);
    int res_status = send(fd, msg.c_str(), msg.size(), 0);
    if (res_status == -1) Utils::print(R, "Error sending message");
    return (1);
}

int Server::handlePassword(int fd, Request req) {
    if (req.getParams().size() < 1) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    if (req.getParams().size() > 1) return (sendMessage(fd, ENOUGH_PARAMS, req));
    if (_clients[fd]->isAuthenticated()) return (sendMessage(fd, USER_ALREADY_AUTHENTICATED, req));
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
    if (!_clients[fd]->getWelcomeMessageDelivered() && _clients[fd]->isRegistered() &&
        _clients[fd]->getNickName().size() > 0) {
        sendMessage(fd, RPL_WELCOME, req);
        sendMessage(fd, RPL_YOURHOST, req);
        sendMessage(fd, RPL_CREATED, req);
        _clients[fd]->setWelcomeMessageDelivered(true);
        return (0);
    }
    return (0);
}

int Server::handleUser(int fd, Request req) {
    bool hasTrailing = false;
    int totalParams = req.getParams().size();
    if (req.getTrailing().size() > 0) hasTrailing = true;
    if (hasTrailing) totalParams++;
    if (totalParams < 4) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    if (totalParams > 4) return (sendMessage(fd, ENOUGH_PARAMS, req));
    if (_clients[fd]->isRegistered()) return (sendMessage(fd, USER_ALREADY_REGISTERED, req));
    _clients[fd]->setRegistration(true);
    _clients[fd]->setUsername(req.getParams()[0]);
    if (hasTrailing)
        _clients[fd]->setRealName(req.getTrailing());
    else
        _clients[fd]->setRealName(req.getParams()[0]);
    if (!_clients[fd]->getWelcomeMessageDelivered() && _clients[fd]->isRegistered() &&
        _clients[fd]->getNickName().size() > 0) {
        sendMessage(fd, RPL_WELCOME, req);
        sendMessage(fd, RPL_YOURHOST, req);
        sendMessage(fd, RPL_CREATED, req);
        _clients[fd]->setWelcomeMessageDelivered(true);
    }
    return (0);
}

int Server::handlePrivateMsg(int fd, Request req) {
    (void)req;
    std::string msg = ":" + _clients[fd]->getNickName() + "!" + _clients[fd]->getUserName() +
                      "@127.0.0.1" + " PRIVMSG " + _clients[fd]->getNickName() + "!" +
                      _clients[fd]->getUserName() + "@127.0.0.1 :Hi\r\n";
    std::cout << msg << std::endl;
    send(fd, msg.c_str(), msg.size(), 0);
    return (0);
}
