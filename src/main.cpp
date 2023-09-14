#include "../incl/Channel.hpp"
#include "../incl/Client.hpp"
#include "../incl/Request.hpp"
#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"


#include "../incl/Channel.hpp"
#include "../incl/Client.hpp"

#define SERVER_NAME "mr.server.com"
#define SERVER_VERSION "v1.0.0"

int channel_playground(void) {
    std::string channelName = "Jurassic Age";
    std::string password = "";
    Channel general(channelName, password);

    std::string ch2 = "Discovery World";
    std::string ch2Pass = "";
    Channel discovery(ch2, ch2Pass);

    Client client1(1);  // Assuming file descriptor 1
    client1.setNickName("Client1");
    Client client2(2);  // Assuming file descriptor 2
    client2.setNickName("client2");
    Client client3(3);  // Assuming file descriptor 3
    client3.setNickName("client3");
    Client client4(4);  // Assuming file descriptor 4
    client4.setNickName("client4");
    Client client5(5); // Assuming file descriptor 5
    client5.setNickName("client5");

    if (general.join(&client1, 1)) {
        std::cout << "Client in " + general.getName() + " !" << std::endl;
    }
    if (general.join(&client2, 2)) {
        std::cout << "Client in " + general.getName() + " !" << std::endl;
    }

    general.setUserLimitMode(on);
    // general.setUserLimit(2);

    if (general.join(&client3, 3)) {
        std::cout << "Client in " + general.getName() + " !" << std::endl;
    }

    general.getMembers();

    general.setTopicMode(on);
    general.setTopic("Jurassic Age");

    general.setPasswordMode(on);
    general.setPassword("123456");

    general.setInviteOnlyMode(on);

    general.setUserLimit(120);


    Utils::print(R, general.getName() + " channel properties: ");
    Utils::print(Y, "Topic: |" + general.getTopic() + "|");
    Utils::print(G, "Password: |" + general.getPassword() + "|");
    Utils::print(P, "User Limit: ", 0);
    std::cout << general.getLimit() << std::endl;
    Utils::print(G, "Invite Only: ", 0);
    std::cout << general.isInviteOnly() << std::endl;

    std::cout << std::endl;

    Utils::print(R, discovery.getName() + " channel properties: ");
    Utils::print(Y, "Topic: |" + discovery.getTopic() + "|");
    Utils::print(G, "Password: |" + discovery.getPassword() + "|");
    Utils::print(P, "User Limit: ", 0);
    std::cout << discovery.getLimit() << std::endl;
    Utils::print(G, "Invite Only: ", 0);
    std::cout << discovery.isInviteOnly() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    general.modifyOpsPrivileges(channelName, "Client2", '+'); // Gives Client2 operator privileges

    if (general.join(&client4, 4)) {
        std::cout << "Client in " + general.getName() + " !" << std::endl;
    }

    general.modifyOpsPrivileges(channelName, "Client1", '-'); // Takes back operator privileges from client1

    if (general.join(&client5, 5)) {
        std::cout << "Client in " + general.getName() + " !" << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    if (discovery.join(&client5, 5)) {
        std::cout << "Client in " + discovery.getName() + " !" << std::endl;
    }
    if (discovery.join(&client1, 1)) {
        std::cout << "Client in " + discovery.getName() + " !" << std::endl;
    }

    return 0;
}

int main(int argc, char **argv) {

    (void) argc;
    (void) argv;

    // channel_playground();
    if (argc != 3) return (Utils::print_error(INVALID_ARGC));
    Server server(SERVER_NAME, SERVER_VERSION, argv[1], argv[2]);
    if (server.valid_args() || server.start_server()) return (1);
    server.run();
};