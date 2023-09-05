#include "../incl/MessageParser.hpp"

Request MessageParser::parseMsg(int fd, std::string msg) {
    std::vector<std::string> tokens;
    std::istringstream stream(msg);
    std::string token;

    std::string prefix = "";
    std::string command = "";
    std::string trailing = "";

    bool trailingExist = false;
    bool trailFound = false;

    while (std::getline(stream, token, ' ')) {
        if (token.size() == 0) continue;
        tokens.push_back(token);
    }

    // for (size_t i = 0; i < tokens.size(); ++i) {
    // 	std::cout << "Token " << i << ": " << tokens[i] << std::endl;
    // }

    if (tokens[0][0] == ':') {
        prefix = tokens[0].substr(1, tokens[0].size() - 1);
        command = tokens[1];
    } else {
        command = tokens[0];
    }

    Request clientRequest = Request();
    clientRequest.setFd(fd);
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
