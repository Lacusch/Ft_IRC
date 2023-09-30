#include "../incl/Utils.hpp"

#include "../incl/Request.hpp"

void Utils::print(Color color, std::string str, bool new_line) {
    if (color == R) std::cout << RED;
    if (color == P) std::cout << PURPLE;
    if (color == G) std::cout << GREEN;
    if (color == B) std::cout << BLUE;
    if (color == Y) std::cout << YELLOW;
    if (color == CGR) std::cout << CURSIVE << GRAY;
    if (color == CB) std::cout << CURSIVE << BLUE;
    if (color == CG) std::cout << CURSIVE << GREEN;
    if (color == CR) std::cout << CURSIVE << RED;
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
    if (err == PASS_TOO_LONG) msg = "Error: password length should be 50 characters or less";
    if (err == CREATING_SOCKET) msg = "Error creating server socket";
    if (err == BINDING_SOCKET) msg = "Error binding server socket";
    if (err == CONFIGURING_SOCKET) msg = "Error configuring server socket";
    if (err == LISTENING_SOCKET) msg = "Error listening server socket";
    if (err == ACCEPTING_CONNECTION) msg = "Error accepting connection";
    if (err == FCNTL_ERROR) msg = "Error making server fd nonblocking";
    if (err == READING_RECV) msg = "Error: reading recv";
    Utils::print(CR, msg);
    return (1);
}

// Trims last 2 chars from irc client (/r/n)
std::string Utils::irc_trim(std::string msg) {
    if (msg.length() == 0) return ("");
    std::string substring = msg.substr(0, msg.length() - 2);
    return (substring);
}

std::vector<std::string> Utils::splitString(std::string str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(str);

    while (std::getline(stream, token, delimiter)) {
        if (token.size() == 0) continue;
        tokens.push_back(token);
    }
    return (tokens);
}

std::string Utils::removeSpace(std::string str) {
    std::string result = "";
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != ' ') result += str[i];
    }
    return (result);
}

Request Utils::parse_msg(int fd, std::string msg) {
    std::vector<std::string> tokens;

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

    tokens = splitString(msg, ' ');

    if (tokens.size() == 0) return (clientRequest);

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
            if (msg[i] == ':' && !trailFound) {
                trailFound = true;
                continue;
            }
            if (trailFound) {
                trailing += msg[i];
            }
        }
    }
    clientRequest.setTrailing(trailing);
    return (clientRequest);
}

std::string Utils::to_upper(std::string str) {
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::toupper(*it);
    }
    return (str);
}

std::string Utils::to_lower(std::string str) {
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        *it = std::tolower(*it);
    }
    return (str);
}

bool Utils::isValidUnsignedInt(const std::string& str) {
    if (str.empty()) return (false);
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i])) {
            return (false);
        }
    }
    return (true);
}

bool Utils::parse_join_msg(Request& req) {
    if (req.getParams().size() < 1) return (false);
    if (req.getParams().size() > 2) return (false);

    std::vector<std::string> channels;
    std::vector<std::string> keys;

    std::string first = req.getParams()[0];
    channels = splitString(first, ',');
    if (channels.size() < 1) return (false);

    if (req.getParams().size() > 1) {
        std::string second = req.getParams()[1];
        keys = splitString(second, ',');
    }

    req.setJoinParams(channels, keys);
    return (true);
}

std::string Utils::to_string(int num) {
    std::stringstream ss;
    ss << num;
    std::string string_val = ss.str();
    return (string_val);
}

void Utils::print_req(Request req) {
    Utils::print(CGR, "--------------------");
    Utils::print(CGR, "timestamp: " + Utils::to_string(std::time(NULL)));
    Utils::print(CGR, "req.fd: " + Utils::to_string(req.getFd()));
    Utils::print(CGR, "req.command: " + req.getCommand());
    if (req.getPrefix().size()) Utils::print(CGR, "req.prefix: " + req.getPrefix());
    for (size_t i = 0; i < req.getParams().size(); i++) {
        Utils::print(CGR, "req.param[" + Utils::to_string(i) + "]: " + req.getParams()[i]);
    }
    if (req.getTrailing().size()) Utils::print(CGR, "req.trailing: " + req.getTrailing());
    Utils::print(CGR, "--------------------");
    Utils::print(CGR, "");
}

void Utils::print_res(int fd, std::string res) {
    Utils::print(CG, "--------------------");
    Utils::print(CG, "timestamp: " + Utils::to_string(std::time(NULL)));
    Utils::print(CG, "res.fd: " + Utils::to_string(fd));
    Utils::print(CG, "res.message: " + res);
    Utils::print(CG, "--------------------");
    Utils::print(CGR, "");
}