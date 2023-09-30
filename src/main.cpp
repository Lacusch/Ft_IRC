#include "../incl/Channel.hpp"
#include "../incl/Client.hpp"
#include "../incl/Request.hpp"
#include "../incl/Server.hpp"
#include "../incl/Shared.hpp"
#include "../incl/Utils.hpp"

#define SERVER_NAME "mr.server.com"
#define SERVER_VERSION "v1.0.0"

Server* server_ptr;

void signal_handler(int signum) {
    server_ptr->clean();
    server_ptr = NULL;
    exit(signum);
}

int main(int argc, char** argv) {
    signal(SIGINT, signal_handler);
    if (argc != 3) return (Utils::print_error(INVALID_ARGC));
    Server server(SERVER_NAME, SERVER_VERSION, argv[1], argv[2]);
    server_ptr = &server;
    if (server.valid_args() || server.start_server()) return (1);
    server.run();
};