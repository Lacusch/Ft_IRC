#include "../incl/Request.hpp"
#include "../incl/Shared.hpp"

struct Status {
    std::string code;
    std::string extra;
    std::string msg;
};

enum Res {
    NOT_ENOUGH_PARAMS,
    ENOUGH_PARAMS,
    USER_ALREADY_REGISTERED,
    INCORRERCT_PASSWORD,
    NO_NICKNAME_GIVEN,
    NICKNAME_IN_USE,
    ERRONEOUS_NICKNAME
};

#pragma once

class Response {
   public:
    static Status create_response(Res res, Request req);
};
