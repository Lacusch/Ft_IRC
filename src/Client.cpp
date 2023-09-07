#include "../incl/Client.hpp"

Client::Client(int fd)
    : _fd(fd),
      _isAuthenticated(false),
      _isRegistered(false),
      _isOperator(false),
      _welcomeMessageDelivered(false),
      _nickName(""),
      _userName(""),
      _realName("") {}

Client::~Client() {}

bool Client::isAuthenticated(void) const { return (_isAuthenticated); }

bool Client::isRegistered(void) const { return (_isRegistered); }

bool Client::isOperator(void) const { return (_isOperator); }

void Client::setRegistration(bool isRegistered) { _isRegistered = isRegistered; }

void Client::setAuthentication(bool isAuthenticated) { _isAuthenticated = isAuthenticated; }

void Client::setOperator(bool isOperator) { _isOperator = isOperator; }

void Client::setUsername(std::string userName) { _userName = userName; }

void Client::setRealName(std::string realName) { _realName = realName; }

void Client::setWelcomeMessageDelivered(bool delivered) { _welcomeMessageDelivered = delivered; }

std::string Client::getNickName(void) const { return (_nickName); }

std::string Client::getUserName(void) const { return (_userName); }

int Client::getFd(void) const { return (_fd); }

bool Client::getWelcomeMessageDelivered(void) const { return (_welcomeMessageDelivered); }

void Client::setNickName(std::string nickname) { _nickName = nickname; }