#include "../incl/Response.hpp"

Status Response::create_response(Res res) {
    Status status;
    if (res == NOT_ENOUGH_PARAMS) {
        status.code = 461;
        status.msg = "Not enough parameters";
    }
    return (status);
}