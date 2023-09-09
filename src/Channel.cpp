#include "../incl/Channel.hpp"

Channel::Channel(std::string & name, std::string & password) 
	:
	_name(name),
	_password(password),
	_topic(""),
	_limit(200),
	_inviteOnly(off),
	_topicMode(on), 
	_passwordMode(on),
	_userLimitMode(off){

	Utils::print(B, "Men this -> " + this->_name + " is not easy");

}

Channel::~Channel() {
	
}

// ------------------------------------------------------------
// --------------------- Channel Methods ----------------------
// ------------------------------------------------------------
bool Channel::join(Client* client, int fd) {

	if (isMember(client, fd))  return false; // Client is already in the channel

	_members[fd] = client; // Add the client to the list of members
	std::cout << "Client " << fd << " " << _members[fd]->getNickName() << " joined the channel." << std::endl; 

	return true;
}

bool Channel::kick(Client* client, int fd) {
	if (!isMember(client, fd))  return false; // Client is not in the channel

	_members.erase(fd); // Eject the client
	return true;
}

bool Channel::invite(Client* client, int fd) {

	if (isMember(client, fd))  return false; // Client is already in the channel
	return true; // Client is not in the channel
}

bool Channel::leave(Client * client, int fd) {

	if (!isMember(client, fd))  return false; // Client is not in the channel
	_members.erase(fd);
	return true;
}

// ------------------------------------------------------------
// ------------------------- Setters --------------------------
// ------------------------------------------------------------
bool Channel::setTopic(const std::string & topic) {
	if (isTopicValid(topic)) {
		if (_topicMode == off) return false;
		_topic = topic;
		return true;
	}
	return false;
}

bool Channel::setPassword(const std::string & password) {
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
// ----------------------- Mode Setters -----------------------
// ------------------------------------------------------------

void Channel::setPasswordMode(State mode) {
	_passwordMode = mode;
}
void Channel::setTopicMode(State mode) {
	_topicMode = mode;
}

void Channel::setInviteOnlyMode(State mode) {
	_inviteOnly = mode;
}

void Channel::setUserLimitMode(State mode) {
	_userLimitMode = mode;
}

// ------------------------------------------------------------
// ------------------------- Getters --------------------------
// ------------------------------------------------------------
const std::string & Channel::getName() const {
	return _name;
}

const std::string & Channel::getTopic() const { return _topic; }

const std::string & Channel::getPassword() const { return _password; }

unsigned int Channel::getLimit() const { return _limit; }

bool Channel::isInviteOnly() const { return _inviteOnly; }

void Channel::getMembers() {
	std::map<int, Client*>::iterator it;
	for (it = _members.begin(); it != _members.end(); ++it) {
		Client* client = it->second;
		std::cout << it->second << "---------->" << client->getNickName() << std::endl;
	}
}


// ------------------------------------------------------------
// -------------------------- Utils ---------------------------
// ------------------------------------------------------------
bool Channel::isMember(Client* client, int fd) const { return (_members.find(fd) != _members.end() && _members.at(fd) == client);}

bool Channel::isTopicValid(const std::string & topic) {
	// check if topic has no line breaks
	if (topic.find('\n') != std::string::npos) return false;
	return true;
}