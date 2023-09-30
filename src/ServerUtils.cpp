#include "../incl/Server.hpp"

bool Server::nickNameInUse(std::string nickname) {
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        if (Utils::to_upper(client->getNickName()) == Utils::to_upper(nickname)) return (true);
    }
    return (false);
}

bool Server::channelExists(std::string channel_name) {
    std::map<std::string, Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it) {
        Channel* channel = it->second;
        if (Utils::to_upper(channel->getName()) == Utils::to_upper(channel_name)) return (true);
    }
    return (false);
}

int Server::getFdFromNickName(std::string nickname) {
    std::map<int, Client*>::iterator it;
    for (it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        if (client->getNickName() == nickname) return (client->getFd());
    }
    return (-1);
}
