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
    Utils::print(R, msg);
    return (1);
}