#include "../incl/Request.hpp"
#include "../incl/Responses.hpp"
#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"

// std::string Server::messageCreator(int fd, Status status) {
//     std::string res = "";
//     res += ":" + this->getName() + " " + status.code + " ";
//     std::string nickName = _clients[fd]->getNickName();
//     if (nickName.length() > 0)
//         res += _clients[fd]->getNickName() + " ";
//     else
//         res += "* ";
//     if (status.extra.length()) res += status.extra + " ";
//     res += ":" + status.msg + "\r\n";
//     Utils::print(P, res);
//     return (res);
// }

std::string client_recipient(Client *client) {
    if (client->getNickName().length() > 0)
        return (client->getNickName());
    else
        return ("*");
};

std::string Server::create_response(int fd, Res res, Request req) {
    // Status status;
    // status.code = "421";
    // status.extra = "";
    // status.msg = "Unknown command";

    std::string msg = "";
    Client *client = _clients[fd];
    if (res == NOT_ENOUGH_PARAMS) {
        msg += ":" + this->getName() + " 461 " + client_recipient(client) +
               " :Not enough parameters\r\n";
    }
    if (res == ENOUGH_PARAMS) {
        msg += ":" + this->getName() + " 461 " + client_recipient(client) +
               " :Too many parameters\r\n";
    }
    if (res == USER_ALREADY_AUTHENTICATED) {
        msg += ":" + this->getName() + " 462 " + client_recipient(client) +
               " :User already authenticated\r\n";
    }
    if (res == USER_ALREADY_REGISTERED) {
        msg += ":" + this->getName() + " 462 " + client_recipient(client) +
               " :User already registered\r\n";
    }
    if (res == INCORRERCT_PASSWORD) {
        msg += ":" + this->getName() + " 464 " + client_recipient(client) +
               " :Incorrect password. Please try again\r\n";
    }
    if (res == NO_NICKNAME_GIVEN) {
        msg += ":" + this->getName() + " 432 " + client_recipient(client) +
               " <empty> :No nickname given\r\n";
    }
    if (res == ERRONEOUS_NICKNAME) {
        msg += ":" + this->getName() + " 432 " + client_recipient(client) +
               (req.getParams()[0].length() ? " " + req.getParams()[0] : "") +
               " :Erroneous nickname\r\n";
    }
    if (res == NICKNAME_IN_USE) {
        msg += ":" + this->getName() + " 433 " + client_recipient(client) +
               (req.getParams().size() ? " " + req.getParams()[0] : "") +
               " :Nickname is already in use\r\n";
    }
    if (res == RPL_WELCOME) {
        msg += ":" + this->getName() + " 001 " + client_recipient(client) +
               " :Welcome to the Internet Relay Network " + client->getNickName() + "!" +
               client->getUserName() + "@127.0.0.1\r\n";
    }
    if (res == RPL_YOURHOST) {
        msg += ":" + this->getName() + " 002 " + client_recipient(client) + " :Your host is " +
               this->getName() + ", running version " + this->getVersion() + "\r\n";
    }
    if (res == RPL_CREATED) {
        msg += ":" + this->getName() + " 003 " + client_recipient(client) +
               " :This server was created " + this->getCreationDate() + "\r\n";
    }
    if (res == NICKNAME_REGISTERED) {
        msg += ":" + (client->getNickName().size() ? client->getNickName() : "*") + "!" +
               (client->getUserName().size() ? client->getUserName() : "*") + "@127.0.0.1" +
               " NICK " + req.getParams()[0] + "\r\n";
        ;
    }
    if (res == SEND_PRIVATE_MESSAGE) {
        msg += ":" + _clients[fd]->getNickName() + "!" + _clients[fd]->getUserName() + "@0.0.0.0" +
               " PRIVMSG " + req.getParams()[0] + " :" + req.getTrailing() + "\r\n";
    }
    Utils::print(Y, msg);
    return (msg);
}