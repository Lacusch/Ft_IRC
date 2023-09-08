#pragma once
#include "Colors.hpp"
#include "Errors.hpp"
#include "Request.hpp"
#include "Shared.hpp"

class Utils {
   public:
    static void print(Color color, std::string str, bool new_line = 1);
    static int print_error(Error err);
    static std::string irc_trim(std::string msg);
    static Request parse_msg(int fd, std::string msg);
    static std::string to_upper(std::string str);
};