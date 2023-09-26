#pragma once

#include "Shared.hpp"
#include "Request.hpp"
#include "Client.hpp"
#include "Utils.hpp"

#define DIE_MIN 2
#define DIE_MAX 10
#define BOT_NICKNAME "bot"
#define BOT_USERNAME "Roll_Die_Bot"
#define BOT_REALNAME "Rolling Die IRC-BOT"

class Bot : public Client {
    public:
        Bot(int fd);
        ~Bot();

        std::string rollDie(unsigned int sides);
};