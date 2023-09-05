#include "../incl/Response.hpp"

#include "../incl/Request.hpp"
#include "../incl/Server.hpp"

Status Response::create_response(Res res, Request req, Client *client) {
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
    if (res == USER_ALREADY_AUTHENTICATED) {
        status.code = "462";
        status.msg = "User already authenticated";
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
    if (res == RPL_WELCOME) {
        status.code = "001";
        std::string extra_text = client->getNickName() + "!" + client->getUserName() + "@127.0.0.1";
        status.msg = "Welcome to the Internet Relay Network " + extra_text;
    }
    if (res == RPL_YOURHOST) {
        status.code = "002";
        status.msg = "Your host is <servername>, running version <ver>";
    }
    if (res == RPL_CREATED) {
        status.code = "003";
        status.msg = "This server was created <date>";
    }
    if (res == NICKNAME_REGISTERED) {
        status.code = "";
        std::string extra_text =
            (client->getNickName().size() ? client->getNickName() : "*") + "!" +
            (client->getUserName().size() ? client->getUserName() : "*") + "@127.0.0.1";
        status.extra = extra_text;
        status.msg = "NICK" + req.getParams()[0];
    }
    return (status);
}