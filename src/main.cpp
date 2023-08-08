#include <iostream>

#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"

int main(int argc, char **argv) {
    if (argc != 3) return (Utils::error(INVALID_ARGC));
    Server server;
    if (!server.valid_args(argv[1], argv[2])) return (1);
    return (0);
}
