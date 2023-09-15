#include "../incl/Channel.hpp"

Channel::Channel(std::string& name, std::string& password)
    : _name(name),
      _password(password),
      _topic(""),
      _limit(200),
      _inviteOnly(off),
      _topicMode(off),
      _passwordMode(on),
      _userLimitMode(off) {
    Utils::print(B, "Men this -> " + this->_name + " is not easy");
}

Channel::~Channel() {}

// ------------------------------------------------------------
//  Channel Methods 
// ------------------------------------------------------------
bool Channel::join(Client* client, int fd) {
    if (isMember(client, fd)) return false;       // Client is already in the channel
    // if (_members.size() >= _limit) return false;  // Channel is full (Check not required here, should be done by the server)

    _members[fd] = client;  // Add the client to the list of members
	Utils::print(P, "Current size of channel: " + this->getName() + " -> ", 0);
	std::cout << _members.size() << std::endl;
    if (_members.size() == 1) {
		_ops[this->getName()].push_back(client->getNickName());
	}

    // For Testing purpose
	Utils::print(Y, "Operators of channel: ");
	for (std::map<std::string, std::vector<std::string> >::iterator it = this->_ops.begin(); it != this->_ops.end(); ++it) {
		std::vector<std::string> savedOperators = it->second;
		while (savedOperators.size() > 0) {
			Utils::print(Y, "|" + it->first + "|" + " operator's name: " + "|" + savedOperators.back() + "|");
			savedOperators.pop_back();
		}
	}
	std::cout << "Client " << fd << " " << _members[fd]->getNickName() << " joined the channel." << std::endl;

	return true;
}

bool Channel::kick(Client* client, int fd) {
    if (!isMember(client, fd)) return false;  // Client is not in the channel
    _members.erase(fd);                       // Eject the client
    return true;
}

bool Channel::invite(Client* client, int fd) {
    if (isMember(client, fd)) return false;  // Client is already in the channel
    return true;
}

bool Channel::leave(Client* client, int fd) {
    if (!isMember(client, fd)) return false;  // Client is not in the channel
    _members.erase(fd);
    return true;
}

bool Channel::modifyOpsPrivileges(const std::string& channel_name, const std::string& nickname, char option) {
	if (option == '+') {
		_ops[channel_name].push_back(nickname);
	}
	else if (option == '-') {
		_ops[channel_name].erase(std::remove(_ops[channel_name].begin(), _ops[channel_name].end(), nickname), _ops[channel_name].end());
	}
	return true;
}




// ----------------------------------------------------------
//  Setters 
// ----------------------------------------------------------

bool Channel::setTopic(const std::string& topic) {
    if (isTopicValid(topic)) {
        _topic = topic;
        return true;
    }
    return false;
}

bool Channel::setPassword(const std::string& password) {
    if (_passwordMode == off) return false;
    _password = password;
    return true;
}

bool Channel::setUserLimit(unsigned int limit) {
    if (_userLimitMode == off) return false;

    if (limit < 2 || limit > 200) return false;
    _limit = limit;
    return true;
}




// ------------------------------------------------------------
//  Mode Setters
// ------------------------------------------------------------

void Channel::setPasswordMode(State mode) { _passwordMode = mode; }

void Channel::setTopicMode(State mode) { _topicMode = mode; }

void Channel::setInviteOnlyMode(State mode) { _inviteOnly = mode; }

void Channel::setUserLimitMode(State mode) { _userLimitMode = mode; }



// ------------------------------------------------------------
//  Getters
// ------------------------------------------------------------

const std::string& Channel::getName() const { return _name; }

const std::string& Channel::getTopic() const { return _topic; }

const std::string& Channel::getPassword() const { return _password; }

unsigned int Channel::getLimit() const { return _limit; }

bool Channel::isInviteOnly() const { return _inviteOnly; }

unsigned int Channel::getChannelSize() const { return (_members.size()); }

bool Channel::getPasswordMode() const { return (_passwordMode); }

bool Channel::getTopicMode() const { return (_topicMode); }

std::map<int, Client*> Channel::getMembersList(void) { return (_members); }

void Channel::printMembers() {
    std::map<int, Client*>::iterator it;
    for (it = _members.begin(); it != _members.end(); ++it) {
        Client* client = it->second;
        std::cout << it->second << "---------->" << client->getNickName() << std::endl;
    }
}

std::vector<std::string> Channel::getOpsList(void) {

    std::map<std::string, std::vector<std::string> >::iterator it;
    std::vector<std::string> savedOperators;

    for (it = _ops.begin(); it != _ops.end(); ++it) {
        if (it->first == this->getName()) {
            savedOperators = it->second;
            break;
        }
    }
    return (savedOperators);
}




// ----------------------------------------------------------
//  Utils
// ----------------------------------------------------------

bool Channel::isMember(Client* client, int fd) const {

    (void) fd;
    std::map<int, Client*>memberList = _members;

    std::map<int, Client*>::iterator it;

    for (it = memberList.begin(); it != memberList.end(); ++it) {
        if (it->second == client) {
            return true;
        }
    }
    return false;
}

bool Channel::isTopicValid(const std::string& topic) {
    // check if topic has no line breaks
    if (topic.find('\n') != std::string::npos) return false;
    if (topic.empty()) return false;

    return true;
}

bool Channel::userIsChannelOp(Client *client, Channel *chName) {

    std::vector<std::string> chOps = chName->getOpsList();
    std::vector<std::string>::iterator it;

    for (it = chOps.begin(); it != chOps.end(); ++it) {
        if (*it == client->getNickName()) return true;
    }

    return false;
}
