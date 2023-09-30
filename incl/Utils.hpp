#pragma once

#include "Colors.hpp"
#include "Errors.hpp"
#include "Request.hpp"
#include "Shared.hpp"

class Utils {
   public:
    static void print(Color color, std::string str, bool new_line = 1);
    static int print_error(Error err);
    static void print_req(Request req);
    static void print_res(int fd, std::string res);
    static std::string to_string(int num);
    static std::string irc_trim(std::string msg);
    static Request parse_msg(int fd, std::string msg);
    static std::string to_upper(std::string str);
    static std::string to_lower(std::string str);
    static bool isValidUnsignedInt(const std::string& str);
    static bool parse_join_msg(Request& req);
    static bool isMultiParamValid(std::string str);
    static std::vector<std::string> splitString(std::string str, char delimiter);
    static std::string removeSpace(std::string str);
};