#include "../incl/Bot.hpp"

Bot::Bot(int fd) : Client(fd, "127.0.0.1") {
    this->_nickName = BOT_NICKNAME;
    this->_userName = BOT_USERNAME;
    this->_realName = BOT_REALNAME;
}

Bot::~Bot() { Utils::print(CR, this->_realName + " destroyed"); }

std::string Bot::rollDie(unsigned int sides) {
    std::srand(static_cast<unsigned>(std::time(NULL)));
    int result = std::rand() % sides + 1;
    std::stringstream ss;
    ss << result;
    std::string roll = ss.str();

    return (roll);
}
