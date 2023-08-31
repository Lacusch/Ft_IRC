#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"

#define SERVER_NAME "MR.SERVER.com"

int main(int argc, char **argv) {
    if (argc != 3) return (Utils::error(INVALID_ARGC));
    Server server(SERVER_NAME, argv[1], argv[2]);
    if (server.valid_args() || server.start_server()) return (1);
    server.run();
};