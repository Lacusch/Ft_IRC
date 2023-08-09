#pragma once
#include <iostream>

#include "./Colors.hpp"
#include "./Errors.hpp"

class Utils {
   public:
    static void print(Color color, std::string str, bool new_line = 1);
    static int error(Error err);
};