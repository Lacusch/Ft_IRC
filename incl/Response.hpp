#include "../incl/Shared.hpp"

struct Status {
    int code;
    std::string msg;
};

enum Res { NOT_ENOUGH_PARAMS };

#pragma once

class Response {
   public:
    static Status create_response(Res res);
};
