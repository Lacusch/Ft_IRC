#include "../incl/Channel.hpp"
#include "../incl/Client.hpp"
#include "../incl/Request.hpp"
#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"

#define SERVER_NAME "mr.server.com"
#define SERVER_VERSION "v1.0.0"

// int channel_playground(void) {
//     std::string channelName = "|--Test Jurassic Channel--|";
//     std::string password = "";
//     Channel general(channelName, password);

//     Client client1(1);  // Assuming file descriptor 1
//     Client client2(2);  // Assuming file descriptor 2
//     Client client3(3);  // Assuming file descriptor 3

//     if (general.join(&client1, 1)) {
//         std::cout << "Client in " + general.getName() + " !" << std::endl;
//     }
//     if (general.join(&client2, 2)) {
//         std::cout << "Client in " + general.getName() + " !" << std::endl;
//     }

//     general.setUserLimitMode(on);
//     general.setUserLimit(2);

//     if (general.join(&client3, 3)) {
//         std::cout << "Client in " + general.getName() + " !" << std::endl;
//     }

//     general.getMembers();

//     general.setTopicMode(on);
//     general.setTopic("Jurassic Age");

//     general.setPasswordMode(on);
//     general.setPassword("123456");

//     general.setInviteOnlyMode(on);

//     general.setUserLimit(120);

//     Utils::print(Y, "Topic: |" + general.getTopic() + "|");
//     Utils::print(G, "Password: |" + general.getPassword() + "|");
//     Utils::print(P, "User Limit: ", 0);
//     std::cout << general.getLimit() << std::endl;
//     Utils::print(G, "Invite Only: ", 0);
//     std::cout << general.isInviteOnly() << std::endl;

//     return 0;
// }

int main(int argc, char **argv) {
    if (argc != 3) return (Utils::print_error(INVALID_ARGC));
    Server server(SERVER_NAME, SERVER_VERSION, argv[1], argv[2]);
    if (server.valid_args() || server.start_server()) return (1);
    server.run();
};