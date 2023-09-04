#include "../incl/Response.hpp"

Status Response::create_response(Res res) {
    Status status;
    status.code = "421";
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
    return (status);
}