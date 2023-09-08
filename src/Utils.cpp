#include "../incl/Utils.hpp"

#include "../incl/Request.hpp"

void Utils::print(Color color, std::string str, bool new_line) {
    if (color == R) std::cout << RED;
    if (color == P) std::cout << PURPLE;
    if (color == G) std::cout << GREEN;
    if (color == B) std::cout << BLUE;
    if (color == Y) std::cout << YELLOW;
    std::cout << str;
    if (new_line) std::cout << std::endl;
    std::cout << RESET;
}

int Utils::print_error(Error err) {
    std::string msg = "Error: unknown error";
    if (err == INVALID_ARGC) msg = "usage: ./ircserv <port> <password>";
    if (err == EMPTY_ARGS) msg = "Error: empty arguments";
    if (err == NUMERIC_PORT) msg = "Error: port number must be int";
    if (err == PORT_RANGE) msg = "Error: please use IRC ports: 6665-6669";
    if (err == CREATING_SOCKET) msg = "Error creating server socket";
    if (err == BINDING_SOCKET) msg = "Error binding server socket";
    if (err == CONFIGURING_SOCKET) msg = "Error configuring server socket";
    if (err == LISTENING_SOCKET) msg = "Error listening server socket";
    Utils::print(R, msg);
    return (1);
}

// Trims last 2 chars from irc client (/r/n)
std::string Utils::irc_trim(std::string msg) {
    if (msg.length() == 0) return ("");
    std::string substring = msg.substr(0, msg.length() - 2);
    return (substring);
}

Request Utils::parse_msg(int fd, std::string msg) {
    std::vector<std::string> tokens;
    std::istringstream stream(msg);
    std::string token;

    Request clientRequest = Request();
    std::string prefix = "";
    std::string command = "";
    std::string trailing = "";

    bool trailingExist = false;
    bool trailFound = false;

    clientRequest.setFd(fd);
    clientRequest.setReceiverFd(fd);
    clientRequest.setPrefix(prefix);
    clientRequest.setCommand(command);
    clientRequest.setTrailing(trailing);

    while (std::getline(stream, token, ' ')) {
        if (token.size() == 0) continue;
        tokens.push_back(token);
    }

    if (tokens.size() == 0) return clientRequest;
    if (tokens[0][0] == ':') {
        prefix = tokens[0].substr(1, tokens[0].size() - 1);
        command = tokens[1];
    } else {
        command = tokens[0];
    }

    clientRequest.setPrefix(prefix);
    clientRequest.setCommand(command);

    size_t index = 1;
    if (prefix.length() != 0) {
        index = 2;
    }
    for (size_t i = index; i < tokens.size(); ++i) {
        if (tokens[i][0] == ':') {
            trailingExist = true;
            break;
        }
        clientRequest.setParams(tokens[i]);
    }
    if (trailingExist) {
        for (size_t i = 1; i < msg.length(); ++i) {
            if (msg[i] == ':') {
                trailFound = true;
                continue;
            }
            if (trailFound) {
                trailing += msg[i];
            }
        }
    }
    clientRequest.setTrailing(trailing);
    return clientRequest;
}

std::string Utils::to_upper(std::string str) {
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::toupper(*it);
    }
    return (str);
}