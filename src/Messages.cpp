#include "../incl/Responses.hpp"
#include "../incl/Server.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

int Server::sendMessage(int fd, Res res, Request req) {
    int sendFd = fd;
    std::string response_msg = this->create_response(fd, res, req);
    if (req.getCommand() == "PRIVMSG") sendFd = req.getReceiverFd();
    int res_status = send(sendFd, response_msg.c_str(), response_msg.size(), 0);
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
    sendMessage(fd, NICKNAME_REGISTERED, req);
    _clients[fd]->setNickName(nickname);
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
    if (req.getParams().size() == 0) return (sendMessage(fd, ERR_NORECIPIENT, req));
    if (req.getTrailing().size() == 0) return (sendMessage(fd, NO_TEXT_TO_SEND, req));
    int recipient_fd = this->getFdFromNickName(req.getParams()[0]);
    if (recipient_fd == -1 && req.getParams()[0][0] == '#') {
        handleChannelMessage(fd, req);
        return (0);
    }
    if (recipient_fd == -1) return (sendMessage(fd, ERR_NOSUCHNICK, req));
    req.setReceiverFd(recipient_fd);
    sendMessage(fd, SEND_PRIVATE_MESSAGE, req);
    return (0);
}

int Server::handleChannelMessage(int fd, Request req) {
    std::string channel_name = req.getParams()[0];
    channel_name = channel_name.substr(1);
    Channel* channel = _channels[channel_name];
    std::map<int, Client*> members = channel->getMembersList();
    std::map<int, Client*>::iterator it;
    for (it = members.begin(); it != members.end(); ++it) {
        Client* idx_member = it->second;
        if (idx_member->getFd() == fd) continue;
        req.setReceiverFd(idx_member->getFd());
        sendMessage(fd, SEND_PRIVATE_MESSAGE, req);
    }
    return (0);
}

int Server::handleJoinChannel(int fd, Request req) {
    if (req.getParams().size() == 0) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    if (req.getParams().size() > 2) return (sendMessage(fd, ENOUGH_PARAMS, req));
    std::string channel_name = req.getParams()[0];
    std::string channel_password = "";
    if (req.getParams().size() == 2) channel_password = req.getParams()[1];
    if (channel_name[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, req));
    channel_name = channel_name.substr(1);
    std::string password = "";
    if (!channelExists(channel_name)) {
        Channel* general = new Channel(channel_name, password);
        _channels[channel_name] = general;
    }
    Channel* channel = _channels[channel_name];
    channel->join(_clients[fd], fd);
    channel->getMembers();
    sendMessage(fd, JOIN_CHANNEL, req);
    return (0);
}
