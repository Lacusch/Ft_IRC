#include "../incl/Channel.hpp"

Channel::Channel(std::string& name, std::string& password)
    : _name(name),
      _password(password),
      _topic(""),
      _limit(CHANNEL_USER_LIMIT),
      _inviteOnly(off),
      _topicMode(off),
      _passwordMode(off) {
    if (!password.empty()) _passwordMode = on;
}

Channel::~Channel() {}

// ------------------------------------------------------------
//  Channel Methods
// ------------------------------------------------------------
bool Channel::join(Client* client, int fd) {
    if (isMember(client, fd)) return (false);

    _members[fd] = client;  // Add the client to the list of members

    Utils::print(P, "Current size of channel: " + this->getName() + " -> ", 0);
    std::cout << _members.size() << std::endl;

    if (_members.size() == 1) {
        _opsList.push_back(client->getNickName());
    }

    printOperators();  // For Testing purpose

    return (true);
}

bool Channel::invite(Client* client, int fd) {
    if (isMember(client, fd)) return (false);
    addToInvitedList(client);
    return (true);
}

bool Channel::leave(Client* client, int fd) {
    if (!isMember(client, fd)) return (false);
    if (userIsChannelOp(client)) {
        this->modifyOpsPrivileges(client, '-');
    }
    _members.erase(fd);
    if (_members.size() == 0) {
        _inviteOnly = off;
        _topicMode = off;
        _passwordMode = off;
        _limit = CHANNEL_USER_LIMIT;
        _topic = "";
    }
    return (true);
}

bool Channel::modifyOpsPrivileges(Client* target, char option) {
    if (!isMember(target, target->getFd())) return (false);

    if (option == '+') {
        _opsList.push_back(target->getNickName());
    } else if (option == '-') {
        _opsList.erase(std::remove(_opsList.begin(), _opsList.end(), target->getNickName()),
                       _opsList.end());
    }
    return (true);
}

void Channel::updateOpsListNick(std::string oldNick, std::string newNick) {
    _opsList.erase(std::remove(_opsList.begin(), _opsList.end(), oldNick), _opsList.end());
    _opsList.push_back(newNick);
}

// ----------------------------------------------------------
//  Setters
// ----------------------------------------------------------

bool Channel::setTopic(const std::string& topic) {
    if (isTopicValid(topic)) {
        _topic = topic;
        return (true);
    }
    return (false);
}

bool Channel::setPassword(const std::string& password) {
    _password = password;
    return (true);
}

bool Channel::setUserLimit(unsigned int limit) {
    if (limit < 2 || limit > CHANNEL_USER_LIMIT) return (false);
    _limit = limit;
    return (true);
}

void Channel::addToInvitedList(Client* client) { _invitedList.push_back(client->getNickName()); }

void Channel::removeFromInvitedList(Client* client) {
    _invitedList.erase(std::remove(_invitedList.begin(), _invitedList.end(), client->getNickName()),
                       _invitedList.end());
}

// ------------------------------------------------------------
//  Mode Setters
// ------------------------------------------------------------

void Channel::setPasswordMode(State mode) { _passwordMode = mode; }

void Channel::setTopicMode(State mode) { _topicMode = mode; }

void Channel::setInviteOnlyMode(State mode) { _inviteOnly = mode; }

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

std::map<int, Client*> Channel::getMembersList(void) const { return (_members); }

std::vector<std::string> Channel::getOpsList(void) const { return (_opsList); }

Client* Channel::getClientByNickName(std::map<int, Client*> clients, std::string nickname) const {
    std::map<int, Client*>::iterator it = clients.begin();
    for (; it != clients.end(); ++it) {
        if (it->second->getNickName() == nickname) return (it->second);
    }

    return (NULL);
}

bool Channel::inInvitedList(Client* client) const {
    std::vector<std::string> invitedList = _invitedList;
    std::vector<std::string>::iterator it;

    for (it = invitedList.begin(); it != invitedList.end(); ++it) {
        if (*it == client->getNickName()) return (true);
    }

    return (false);
}

// ----------------------------------------------------------
//  Utils
// ----------------------------------------------------------

bool Channel::isMember(Client* client, int fd) const {
    (void)fd;
    std::map<int, Client*> memberList = _members;

    std::map<int, Client*>::iterator it;

    for (it = memberList.begin(); it != memberList.end(); ++it) {
        if (it->second == client) return (true);
    }

    return (false);
}

bool Channel::isTopicValid(const std::string& topic) {
    // check if topic has no line breaks
    if (topic.find('\n') != std::string::npos) return (false);
    if (topic.empty()) return (false);
    std::vector<std::string> strArr = Utils::splitString(topic, ' ');
    if (strArr.size() == 0) return (false);
    bool tabsOnly = true;
    for (size_t i = 0; i < strArr.size(); ++i) {
        if (Utils::splitString(strArr[i], '\t').size() > 0) tabsOnly = false;
    }
    strArr.clear();
    if (tabsOnly) return false;

    return (true);
}

bool Channel::userIsChannelOp(Client* client) {
    std::vector<std::string> chOps = this->getOpsList();
    std::vector<std::string>::iterator it;

    for (it = chOps.begin(); it != chOps.end(); ++it) {
        if (*it == client->getNickName()) return (true);
    }

    return (false);
}

void Channel::printMembers() {
    std::map<int, Client*>::iterator it;
    for (it = _members.begin(); it != _members.end(); ++it) {
        Client* client = it->second;
        std::cout << it->second << "---------->" << client->getNickName() << std::endl;
    }
}

void Channel::printOperators() {
    Utils::print(B, "Operators of channel: ");

    for (std::vector<std::string>::iterator it = this->_opsList.begin(); it != this->_opsList.end();
         ++it) {
        Utils::print(Y, "|" + this->getName() + " Operator's name: |" + *it + "|");
    }
}
