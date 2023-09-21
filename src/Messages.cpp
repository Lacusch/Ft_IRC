#include "../incl/Responses.hpp"
#include "../incl/Server.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

int Server::sendMessage(int fd, Res res, Request req) {
    int sendFd = fd;
    std::string response_msg = this->create_response(fd, res, req);
    sendFd = req.getReceiverFd();
    int res_status = send(sendFd, response_msg.c_str(), response_msg.size(), 0);
    Utils::print(G, response_msg);
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

void Server::updateOpNickName(Client *client, std::string newNickName) {
    
    std::map<std::string, Channel *> channels = this->_channels;

    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++) {
        std::vector<std::string> opsList = it->second->getOpsList();
        for (std::vector<std::string>::iterator it2 = opsList.begin(); it2 != opsList.end(); it2++) {
            if (*it2 == client->getNickName()) {
                Utils::print(B, *it2 + " I am updating my nick name to " + newNickName);

                std::string oldNickName = *it2;
                it->second->updateOpsListNick(oldNickName, newNickName);
            }
        }
    }
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
    updateOpNickName(_clients[fd], nickname);
    sendMessage(fd, NICKNAME_REGISTERED, req);
    _clients[fd]->setNickName(nickname);
    _clients[fd]->setHasNickname(true);
    if (!_clients[fd]->getWelcomeMessageDelivered() && _clients[fd]->isRegistered() &&
        _clients[fd]->hasNickname()) {
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
        _clients[fd]->hasNickname()) {
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
    if (recipient_fd == -1 && req.getParams()[0][0] == '#') return (handleChannelMessage(fd, req));
    if (recipient_fd == -1) return (sendMessage(fd, ERR_NOSUCHNICK, req));
    req.setReceiverFd(recipient_fd);
    sendMessage(fd, SEND_PRIVATE_MESSAGE, req);
    return (0);
}

int Server::handlePing(int fd, Request req) {
    (void)req;
    (void)fd;
    return (0);
}

int Server::handleWho(int fd, Request req) {
    (void)req;
    (void)fd;
    return (0);
}

int Server::handleChannelMessage(int fd, Request req) {
    std::string channel_name = Utils::to_lower(req.getParams()[0]);
    channel_name = channel_name.substr(1);
    Channel *channel = _channels[channel_name];
    std::map<int, Client *> members = channel->getMembersList();
    std::map<int, Client *>::iterator it;
    for (it = members.begin(); it != members.end(); ++it) {
        Client *idx_member = it->second;
        if (idx_member->getFd() == fd) continue;
        req.setReceiverFd(idx_member->getFd());
        sendMessage(fd, SEND_PRIVATE_MESSAGE, req);
    }
    return (0);
}

int Server::broadcastChannel(int fd, Request req, Channel *channel, Res res) {
    std::map<int, Client *> members = channel->getMembersList();
    std::map<int, Client *>::iterator it;
    for (it = members.begin(); it != members.end(); ++it) {
        Client *idx_member = it->second;
        req.setReceiverFd(idx_member->getFd());
        sendMessage(fd, res, req);
    };
    return (0);
}

int Server::sendRegisteredUsers(int fd, Request req, Channel *channel) {
    std::map<int, Client *> members = channel->getMembersList();
    std::map<int, Client *>::iterator it;
    for (it = members.begin(); it != members.end(); ++it) {
        Client *idx_member = it->second;
        std::string msg = ":" + this->getName() + " 353 " + idx_member->getNickName() + " = " +
                          req.getParams()[0] + " :" + idx_member->getNickName() + "\r\n";
        Utils::print(G, msg);
        send(fd, msg.c_str(), msg.size(), 0);
    };
    if (members.size() > 1) {
        std::string end_list = ":" + this->getName() + " 366 " + _clients[fd]->getNickName() +
                               " #" + channel->getName() + " :End of NAMES list\r\n";
        Utils::print(G, end_list);
        send(fd, end_list.c_str(), end_list.size(), 0);
    }
    return (0);
}

int Server::handleJoinChannel(int fd, Request req) {
    Utils::parse_join_msg(req);
    std::vector<std::pair<std::string, std::string> > joinCommands = req.getJoinParams();
    if (joinCommands.empty()) sendMessage(fd, NOT_ENOUGH_PARAMS, req);
    std::vector<std::pair<std::string, std::string> >::iterator it;
    for (it = joinCommands.begin(); it != joinCommands.end(); ++it)
        handleSingleChannel(fd, req, Utils::to_lower(it->first), it->second);
    return (0);
}

Channel *Server::getChannel(std::string channel_name) const {
    std::map<std::string, Channel *>::iterator it;
    std::map<std::string, Channel *> channels = _channels;
    for (it = channels.begin(); it != channels.end(); ++it) {
        if (Utils::to_upper(channel_name) == Utils::to_upper(it->first)) return (it->second);
    }
    return (NULL);
}

int Server::handleSingleChannel(int fd, Request req, std::string channel, std::string key) {
    Request request;
    request.setCommand(req.getCommand());
    request.setFd(req.getFd());
    request.setReceiverFd(req.getReceiverFd());
    request.setPrefix(req.getPrefix());
    request.setTrailing(req.getTrailing());
    request.setParams(channel);
    if (!key.empty()) request.setParams(key);
    if (channel[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, request));
    channel = channel.substr(1);
    if (!channelExists(channel)) {
        std::string pass = (key.empty() ? "" : key);
        Channel *general = new Channel(channel, pass);
        _channels[channel] = general;
    }
    Channel *ch = getChannel(channel);
    // TODO null case
    if (!ch->isMember(_clients[fd], fd) && ch->isInviteOnly()) return (sendMessage(fd, ERR_INVITEONLYCHAN, request));
    if (ch->getPassword() != key && ch->getPasswordMode())
        return (sendMessage(fd, ERR_BADCHANNELKEY, request));
    if (ch->getMembersList().size() >= ch->getLimit())
        return (sendMessage(fd, ERR_CHANNELISFULL, req));
    if (!ch->join(_clients[fd], fd)) return (sendMessage(fd, ERR_USERONCHANNEL, req));
    broadcastChannel(fd, request, ch, JOIN_CHANNEL);
    sendRegisteredUsers(fd, request, ch);
    return (0);
}

int Server::handleMode(int fd, Request req) {
    std::vector<std::string> params = req.getParams();
    if (params.size() == 1) {
        std::string channel_name = Utils::to_lower(params[0].substr(1));
        Channel *channel = _channels[channel_name];
        std::vector<std::string> OpsList = channel->getOpsList();
        for (std::vector<std::string>::iterator it = OpsList.begin(); it != OpsList.end(); ++it) {
            std::string msg =
                ":" + this->getName() + " MODE " + "#" + channel_name + " +o " + *it + "\r\n";
            Utils::print(G, msg);
            send(fd, msg.c_str(), msg.size(), 0);
        }
        std::string topic = channel->getTopic();
        bool topiChangeForOperators = !channel->getTopicMode();
        std::string mode_msg = ":" + this->getName() + " MODE " + "#" + channel_name + " " +
                               (topiChangeForOperators ? "+t" : "-t") + "\r\n";
        Utils::print(G, mode_msg);
        if (!topic.empty()) {
            req.setTrailing(topic);
            (sendMessage(fd, TOPIC_SET, req));
        }
        send(fd, mode_msg.c_str(), mode_msg.size(), 0);
        return (0);
    }
    if (req.getParams().size() == 2) {
        if (req.getParams()[1] == "b") return (0);
    }
    if (req.getParams().size() < 2 || req.getParams().size() > 3)
        return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    std::string channel_name = Utils::to_lower(req.getParams()[0]);
    if (channel_name[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, req));

    channel_name = channel_name.substr(1);

    if (!channelExists(channel_name)) return (sendMessage(fd, ERR_NOSUCHCHANNEL, req));

    Channel *ch = _channels[channel_name];

    // Check if the user is a channel operator
    if (!ch->userIsChannelOp(_clients[fd])) return (sendMessage(fd, ERR_CHANOPRIVSNEEDED, req));
    return (this->handleChannelMode(fd, req, ch));
}

Res Server::handleOperatorMode(Client *target, Request req, Channel *ch) {
    std::string mode = req.getParams()[1];
    if (req.getParams().size() != 3) return (NOT_ENOUGH_PARAMS);

    if (mode[0] == '+') {
        if (ch->userIsChannelOp(target)) return (ERR_USERSDONTMATCH);
        if (!ch->modifyOpsPrivileges(target, '+')) return (ERR_USERNOTINCHANNEL);
    } else if (mode[0] == '-') {
        if (!ch->modifyOpsPrivileges(target, '-')) return (ERR_USERNOTINCHANNEL);
    }

    return (RPL_CHANNELMODEIS);
}

Res Server::handleTopicMode(Request req, Channel *ch) {
    std::string mode = req.getParams()[1];
    if (req.getParams().size() != 2) return (NOT_ENOUGH_PARAMS);
    if (mode[0] == '+') {
        ch->setTopicMode(off);  // only operators can set topic when it's off
    } else if (mode[0] == '-') {
        ch->setTopicMode(on);  // all users can set topic when it's on
    }
    return (RPL_CHANNELMODEIS);
}

Res Server::handleInviteOnlyMode(Request req, Channel *ch) {
    std::string mode = req.getParams()[1];
    if (req.getParams().size() != 2) {
        return (NOT_ENOUGH_PARAMS);
    }
    if (mode[0] == '+') {
        ch->setInviteOnlyMode(on);
    } else if (mode[0] == '-') {
        ch->setInviteOnlyMode(off);
    }

    return (RPL_CHANNELMODEIS);
}

Res Server::handleUserLimitMode(Request req, Channel *ch) {
    std::string mode = req.getParams()[1];
    if (req.getParams().size() == 3) {
        if (mode[0] == '+') {
            if (!Utils::isValidUnsignedInt(req.getParams()[2])) return (NOT_ENOUGH_PARAMS);
            unsigned int limit = static_cast<unsigned int>(std::atoi(req.getParams()[2].c_str()));
            if (!ch->setUserLimit(limit)) {
                return (NOT_ENOUGH_PARAMS);
            }
        } else
            return (NOT_ENOUGH_PARAMS);
    } else {
        if (req.getParams().size() != 2) {
            return (NOT_ENOUGH_PARAMS);
        }
        if (mode[0] == '-') {
            ch->setUserLimit(CHANNEL_USER_LIMIT);
        } else
            return (NOT_ENOUGH_PARAMS);
    }

    return (RPL_CHANNELMODEIS);
}

Res Server::handleKeyMode(Request req, Channel *ch) {
    std::string mode = req.getParams()[1];
    if (req.getParams().size() == 3) {
        if (mode[0] == '+') {
            ch->setPasswordMode(on);
            ch->setPassword(req.getParams()[2]);
        } else
            return (NOT_ENOUGH_PARAMS);
    } else {
        if (req.getParams().size() != 2) {
            return (NOT_ENOUGH_PARAMS);
        }
        if (mode[0] == '-') {
            ch->setPasswordMode(off);
        } else
            return (NOT_ENOUGH_PARAMS);
    }

    return (RPL_CHANNELMODEIS);
}

int Server::handleChannelMode(int fd, Request req, Channel *ch) {
    if (req.getParams().size() < 2) {
        return sendMessage(fd, NOT_ENOUGH_PARAMS, req);
    }
    std::string mode = req.getParams()[1];
    if (mode.length() != 2) return (sendMessage(fd, ERR_UNKNOWNMODE, req));

    if (mode[0] != '+' && mode[0] != '-') return (sendMessage(fd, ERR_UNKNOWNMODE, req));

    mode = Utils::to_upper(mode);
    Res response;

    Client *target = NULL;
    switch (mode[1]) {
        case 'O':
            if (req.getParams()[2].empty()) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
            target = ch->getClientByNickName(this->getClientsList(), req.getParams()[2]);
            if (target == NULL && req.getParams().size() == 3)
                return (sendMessage(fd, ERR_NOSUCHNICK, req));
            response = handleOperatorMode(target, req, ch);
            break;
        case 'K':
            response = handleKeyMode(req, ch);
            break;
        case 'L':
            response = handleUserLimitMode(req, ch);
            break;
        case 'T':
            response = handleTopicMode(req, ch);
            break;
        case 'I':
            response = handleInviteOnlyMode(req, ch);
            break;
        default:
            response = ERR_UNKNOWNMODE;
            break;
    }
    return (broadcastChannel(fd, req, ch, response));
}

int Server::handleKick(int fd, Request req) {
    if (req.getParams().size() != 2) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    std::string channel_name = req.getParams()[0];
    if (channel_name[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, req));

    channel_name = channel_name.substr(1);

    if (!channelExists(channel_name)) return (sendMessage(fd, ERR_NOSUCHCHANNEL, req));

    Channel *ch = _channels[channel_name];

    if (!ch->isMember(_clients[fd], fd)) return (sendMessage(fd, ERR_NOTONCHANNEL, req));

    if (!ch->userIsChannelOp(_clients[fd])) return (sendMessage(fd, ERR_CHANOPRIVSNEEDED, req));

    if (req.getParams()[1].empty()) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));

    Client *target = ch->getClientByNickName(this->getClientsList(), req.getParams()[1]);
    if (target == NULL) return (sendMessage(fd, ERR_NOSUCHNICK, req));

    int targetFd = target->getFd();
    broadcastChannel(fd, req, ch, RPL_KICKED);
    if (!ch->kick(target, targetFd)) return (sendMessage(fd, ERR_USERNOTINCHANNEL, req));

    return (0);
}

int Server::handleInvite(int fd, Request req) {
    if (req.getParams().size() != 2) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    std::string channel_name = Utils::to_lower(req.getParams()[1]);
    if (channel_name[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, req));

    channel_name = channel_name.substr(1);

    if (!channelExists(channel_name)) return (sendMessage(fd, ERR_NOSUCHCHANNEL, req));

    Channel *ch = _channels[channel_name];

    if (!ch->isMember(_clients[fd], fd)) return (sendMessage(fd, ERR_NOTONCHANNEL, req));

    if (!ch->userIsChannelOp(_clients[fd]) && ch->isInviteOnly()) return (sendMessage(fd, ERR_CHANOPRIVSNEEDED, req));

    Client *target = ch->getClientByNickName(this->getClientsList(), req.getParams()[0]);
    if (target == NULL) return (sendMessage(fd, ERR_NOSUCHNICK, req));

    if (!target->isRegistered()) return (1);

    int targetFd = target->getFd();
    if (!ch->invite(target, targetFd)) return (sendMessage(fd, ERR_USERONCHANNEL, req));

    if (ch->getChannelSize() >= ch->getLimit()) return (sendMessage(fd, ERR_CHANNELISFULL, req));

    return (sendMessage(fd, RPL_INVITING, req));
}

int Server::handleTopic(int fd, Request req) {
    if (req.getParams().size() != 1) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    std::string channel_name = Utils::to_lower(req.getParams()[0]);
    if (channel_name[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, req));

    channel_name = channel_name.substr(1);

    if (!channelExists(channel_name)) return (sendMessage(fd, ERR_NOSUCHCHANNEL, req));

    Channel *ch = _channels[channel_name];

    if (!ch->isMember(_clients[fd], fd)) return (sendMessage(fd, ERR_NOTONCHANNEL, req));

    if (req.getTrailing().empty()) {
        req.setTrailing(ch->getTopic());
        return (ch->getTopic().empty()) ? (sendMessage(fd, RPL_NOTOPIC, req))
                                        : (sendMessage(fd, TOPIC_SET, req));
    }

    if (ch->getTopicMode() == off) {
        if (!ch->userIsChannelOp(_clients[fd])) return (sendMessage(fd, ERR_CHANOPRIVSNEEDED, req));
    }
    std::string topic = req.getTrailing();
    ch->setTopic(req.getTrailing());

    return (broadcastChannel(fd, req, ch, TOPIC_SET));
}

int Server::handlePart(int fd, Request req) {
    if (req.getParams().size() != 1) return (sendMessage(fd, NOT_ENOUGH_PARAMS, req));
    std::string channel_name = Utils::to_lower(req.getParams()[0]);
    if (channel_name[0] != '#') return (sendMessage(fd, BAD_CHANNEL_STRUCTURE, req));

    channel_name = channel_name.substr(1);

    if (!channelExists(channel_name)) return (sendMessage(fd, ERR_NOSUCHCHANNEL, req));

    Channel *ch = _channels[channel_name];

    if (!ch->leave(_clients[fd], fd)) return (sendMessage(fd, ERR_NOTONCHANNEL, req));

    broadcastChannel(fd, req, ch, RPL_PARTED);
    return (sendMessage(fd, RPL_PARTED, req));
}
