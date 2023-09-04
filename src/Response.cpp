#include "../incl/Response.hpp"

#include "../incl/Request.hpp"

Status Response::create_response(Res res, Request req) {
    Status status;
    status.code = "421";
    status.extra = "";
    status.msg = "Unknown command";
    if (res == NOT_ENOUGH_PARAMS) {
        status.code = "461";
        status.msg = "Not enough parameters";
    }
    if (res == ENOUGH_PARAMS) {
        status.code = "461";
        status.msg = "Too many parameters";
    }
    if (res == USER_ALREADY_REGISTERED) {
        status.code = "462";
        status.msg = "User already registered";
    }
    if (res == INCORRERCT_PASSWORD) {
        status.code = "464";
        status.msg = "Incorrect password. Please try again.";
    }
    if (res == NO_NICKNAME_GIVEN) {
        status.code = "432";
        status.extra = "<empty>";
        status.msg = "No nickname given";
    }
    if (res == ERRONEOUS_NICKNAME) {
        status.code = "432";
        status.extra = req.getParams()[0];
        status.msg = "Erroneous nickname";
    }
    if (res == NICKNAME_IN_USE) {
        status.code = "433";
        status.extra = req.getParams()[0];
        status.msg = "Nickname is already in use";
    }
    return (status);
}