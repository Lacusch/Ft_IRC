#include "../incl/Utils.hpp"

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

int Utils::error(Error err) {
    std::string msg = "Error: unknown error";
    if (err == INVALID_ARGC) msg = "usage: ./ircserv <port> <password>";
    if (err == EMPTY_ARGS) msg = "Error: empty arguments";
    if (err == NUMERIC_PORT) msg = "Error: port number must be int";
    if (err == PORT_RANGE) msg = "Error: please use IRC ports: 6665-6669";
    if (err == CREATING_SOCKET) msg = "Error creating server socket";
    if (err == BINDING_SOCKET) msg = "Error binding server socket";
    if (err == LISTENING_SOCKET) msg = "Error listening server socket";
    Utils::print(R, msg);
    return (1);
}