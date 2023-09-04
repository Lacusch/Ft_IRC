#include "../incl/Shared.hpp"

struct Status {
    std::string code;
    std::string msg;
};

enum Res { NOT_ENOUGH_PARAMS, ENOUGH_PARAMS, USER_ALREADY_REGISTERED, INCORRERCT_PASSWORD };

#pragma once

class Response {
   public:
    static Status create_response(Res res);
};
