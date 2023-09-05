#pragma once
#include <sstream>

#include "../incl/Request.hpp"
#include "./Shared.hpp"

/*
 * MessageParser is a class that parses the initial messages received from the server
 */
class MessageParser {
   private:
    MessageParser();
    ~MessageParser();

   public:
    static Request parseMsg(int fd, std::string msg);
};