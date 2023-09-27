#include "../incl/Request.hpp"
#include "../incl/Responses.hpp"
#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"

std::string client_recipient(Client *client) {
    if (client->getNickName().length() > 0)
        return (client->getNickName());
    else
        return ("*");
};

std::string Server::create_response(int fd, Res res, Request req) {
    std::string msg = "";
    Client *client = _clients[fd];

    switch (res) {
        case NOT_ENOUGH_PARAMS:
            msg = ":" + this->getName() + " 461 " + client_recipient(client) +
                  " :Not enough parameters\r\n";
            break;
        case ENOUGH_PARAMS:
            msg = ":" + this->getName() + " 461 " + client_recipient(client) +
                  " :Too many parameters\r\n";
            break;
        case USER_ALREADY_AUTHENTICATED:
            msg = ":" + this->getName() + " 462 " + client_recipient(client) +
                  " :User already authenticated\r\n";
            break;
        case USER_ALREADY_REGISTERED:
            msg = ":" + this->getName() + " 462 " + client_recipient(client) +
                  " :User already registered\r\n";
            break;
        case INCORRERCT_PASSWORD:
            msg = ":" + this->getName() + " 464 " + client_recipient(client) +
                  " :Incorrect password. Please try again\r\n";
            break;
        case PROVIDE_PASS:
            msg = ":" + this->getName() + " 464 " + client_recipient(client) +
                  " :Please provide the server password\r\n";
            break;
        case REGISTER_FIRST:
            msg = ":" + this->getName() + " 464 " + client_recipient(client) +
                  " :You must register before\r\n";
            break;
        case NO_NICKNAME_GIVEN:
            msg = ":" + this->getName() + " 432 " + client_recipient(client) +
                  " <empty> :No nickname given\r\n";
            break;
        case ERRONEOUS_NICKNAME:
            msg = ":" + this->getName() + " 432 " + client_recipient(client) +
                  (req.getParams()[0].length() ? " " + req.getParams()[0] : "") +
                  " :Erroneous nickname\r\n";
            break;
        case ERR_CHANNELNAME_LENGTH:
            msg = ":" + this->getName() + " 401 " + client_recipient(client) +
                  " <channel_name>  : Channel length should be less than 200 chars" + "\r\n";
            break;
        case NICKNAME_IN_USE:
            msg = ":" + this->getName() + " 433 " + client_recipient(client) +
                  (req.getParams().size() ? " " + req.getParams()[0] : "") +
                  " :Nickname is already in use\r\n";
            break;
        case RPL_WELCOME:
            msg = ":" + this->getName() + " 001 " + client_recipient(client) +
                  " :Welcome to the Internet Relay Network " + client->getNickName() + "!" +
                  client->getUserName() + "@127.0.0.1\r\n";
            break;
        case RPL_YOURHOST:
            msg = ":" + this->getName() + " 002 " + client_recipient(client) + " :Your host is " +
                  this->getName() + ", running version " + this->getVersion() + "\r\n";
            break;
        case RPL_CREATED:
            msg = ":" + this->getName() + " 003 " + client_recipient(client) +
                  " :This server was created " + this->getCreationDate() + "\r\n";
            break;
        case NICKNAME_REGISTERED:
            msg = ":" + (client->getNickName().size() ? client->getNickName() : "*") + "!" +
                  (client->getUserName().size() ? client->getUserName() : "*") + "@127.0.0.1" +
                  " NICK " + req.getParams()[0] + "\r\n";
            break;
        case SEND_PRIVATE_MESSAGE:
            msg = ":" + _clients[fd]->getNickName() + "!" + _clients[fd]->getUserName() +
                  "@127.0.0.1" + " PRIVMSG " + req.getParams()[0] + " :" + req.getTrailing() +
                  "\r\n";
            break;
        case UNKNWON_COMMAND:
            msg = ":" + this->getName() + " 421 " + client_recipient(client) + " " +
                  (req.getCommand().size() ? req.getCommand() : "") + " :Unknown command" + "\r\n";
            break;
        case ERR_NORECIPIENT:
            msg = ":" + this->getName() + " 401 " + client_recipient(client) +
                  "<empty> :No recipient given" + "\r\n";
            break;
        case NO_TEXT_TO_SEND:
            msg = ":" + this->getName() + " 401 " + client_recipient(client) +
                  " <empty> :No text to send" + "\r\n";
            break;
        case ERR_NOSUCHNICK:
            msg = ":" + this->getName() + " 401 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :No such nick/channel" + "\r\n";
            break;
        case JOIN_CHANNEL:
            msg = ":" + _clients[fd]->getNickName() + "!" + _clients[fd]->getUserName() +
                  "@127.0.0.1 " + "JOIN " + req.getParams()[0] + "\r\n";
            break;
        case BAD_CHANNEL_STRUCTURE:
            msg = ":" + this->getName() + " 401 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :Channel should begin with #\r\n";
            break;
        case TOPIC_SET:
            msg = ":" + this->getName() + " 332 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :" + req.getTrailing() + "\r\n";
            break;
        case WHO_CHANNEL:
            msg = ":" + _clients[fd]->getNickName() + "!" + _clients[fd]->getUserName() +
                  "@127.0.0.0 " + this->getName() + " " + _clients[fd]->getNickName() + " 1 \r\n";
            break;
        case ERR_UNKNOWNMODE:
            msg = ":" + this->getName() + " 472 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :is unknown mode " +
                  (req.getParams().size() > 1 ? req.getParams()[1] : "") + "\r\n";
            break;
        case ERR_USERNOTINCHANNEL:
            msg = ":" + this->getName() + " 441 " + req.getParams()[1] + " " + req.getParams()[0] +
                  " :They aren't on that channel" + "\r\n";
            break;
        case ERR_CHANOPRIVSNEEDED:
            msg = ":" + this->getName() + " 482 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :You're not channel operator" + "\r\n";
            break;
        case ERR_NOTREGISTERED:
            msg = ":" + this->getName() + " 451 " + client_recipient(client) +
                  " :You have not registered\r\n";
            break;
        case RPL_CHANNELMODEIS:
            msg = ":" + this->getName() + " MODE " +
                  (req.getParams().size() == 2
                       ? req.getParams()[0] + " " + req.getParams()[1]
                       : req.getParams()[0] + " " + req.getParams()[1] + " " + req.getParams()[2]) +
                  "\r\n";
            break;
        case ERR_NOTONCHANNEL:
            msg = ":" + this->getName() + " 442 " +
                  (req.getParams()[0][0] == '#' ? req.getParams()[0]
                   : req.getParams().size() > 1 ? req.getParams()[1]
                                                : "") +
                  " :You're not on that channel" + "\r\n";
            break;
        case RPL_KICKED:
            msg = "KICK " + req.getParams()[0] + " " + req.getParams()[1] + " :" +
                  (req.getTrailing().empty() ? "" : req.getTrailing()) + "\r\n";
            break;
        case ERR_USERONCHANNEL:
            msg = ":" + this->getName() + " 443 " + client_recipient(client) + " " +
                  req.getParams()[0] + ": channel " + "\r\n";
            break;
        case RPL_INVITING:
            msg = ":" + this->getName() + " 341 " + client_recipient(client) + " " +
                  req.getParams()[0] + " " + req.getParams()[1] + "\r\n";
            break;
        case RPL_NOTOPIC:
            msg = ":" + this->getName() + " 331 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :No topic is set" + "\r\n";
            break;
        case RPL_TOPIC:
            msg = ":" + this->getName() + " 332 " + client_recipient(client) + " " +
                  req.getParams()[0] + " : " + req.getParams()[1] + "\r\n";
            break;
        case ERR_CHANNELISFULL:
            msg = ":" + this->getName() + " 471 " + client_recipient(client) + " " +
                  (req.getParams()[0][0] == '#' ? req.getParams()[0] : req.getParams()[1]) +
                  " :Cannot join channel (+l)" + "\r\n";
            break;
        case ERR_INVITEONLYCHAN:
            msg = ":" + this->getName() + " 473 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :Cannot join channel (+i)" + "\r\n";
            break;
        case ERR_BADCHANNELKEY:
            msg = ":" + this->getName() + " 475 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :Cannot join channel (+k)\r\n";
            break;
        case ERR_USERSDONTMATCH:
            msg = ":" + this->getName() + " 502 " + client_recipient(client) + " " +
                  req.getParams()[0] + " :" + req.getParams()[2] +
                  " is already an operator"
                  "\r\n";
            break;
        case ERR_NOSUCHCHANNEL:
            msg = ":" + this->getName() + " 403 " + client_recipient(client) + " " +
                  (req.getParams()[0][0] == '#' ? req.getParams()[0]
                   : req.getParams().size() > 1 ? req.getParams()[1]
                                                : "") +
                  " :No such channel" + "\r\n";
            break;
        case RPL_PARTED:
            msg = ":" + client->getNickName() + "!" + client->getUserName() + "@127.0.0.1 " +
                  " PART " + req.getParams()[0] + " :Leaving the channel" + "\r\n";
            break;
        case ERR_NICKNAME_FOR_BOT:
            msg = ":" + this->getName() + " 433 " + client_recipient(client) +
                  (req.getParams().size() ? " " + req.getParams()[0] : "") +
                  " :Cannot use nickname, it is in use by BOT \r\n";
            break;
        case BOT_USAGE:
            msg = ":" + this->getName() + " 461 " + client_recipient(client) +
                  " : Usage: <ROLL> <die_number_of_sides> ( min 2, max 10 )\r\n";
            break;
        case PONG_RESPONSE:
            msg = "PONG " + req.getTrailing() + "\r\n";
            break;
        default:
            msg = ":" + this->getName() + " 421 " + client_recipient(client) + " :Unknown command" +
                  "\r\n";
            break;
    }
    return (msg);
}