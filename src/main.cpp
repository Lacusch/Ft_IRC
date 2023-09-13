#include "../incl/Channel.hpp"
#include "../incl/Client.hpp"
#include "../incl/Request.hpp"
#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"

#define SERVER_NAME "mr.server.com"
#define SERVER_VERSION "v1.0.0"

// // Global variables for testing
// std::string ch1 = "general";
// std::string ch1Pass = "";
// Channel general(ch1, ch1Pass);

// std::string ch2 = "Discovery";
// std::string ch2Pass = "";
// Channel discovery(ch2, ch2Pass);


// bool isValidUnsignedInt(const std::string& str) {
//     if (str.empty()) {
//         return false;
//     }

//     for (size_t i = 0; i < str.length(); ++i) {
//         if (!std::isdigit(str[i])) {
//             return false;
//         }
//     }

//     return true;
// }


// bool userIsChannelOp(Client *client, Channel *chName) {
//     std::map<std::string, std::vector<std::string> > chOps = chName->getOpsList();
//     std::map<std::string, std::vector<std::string> >::iterator it;
//     for (it = chOps.begin(); it != chOps.end(); ++it) {
//         std::vector<std::string> savedOperators = it->second;
//         while (savedOperators.size() > 0) {
//             if (savedOperators.back() == client->getNickName()) return true;
//             savedOperators.pop_back();
//         }
//     }
//     return false;
// }


// int handleOperatorMode(Client *target, Request req, Channel *ch) {

//     std::string mode = req.getParams()[1];
//     if (req.getParams().size() != 3) {
//         std::cout << "Invalid mode 'o' command." << std::endl;
//         Utils::print(R, "Response  :server.example.com 461 yourNickName MODE :Not enough parameters");
//         return (NOT_ENOUGH_PARAMS);
//     }
//     if (mode[0] == '+') {
//         std::cout << "MODE #channel +o userNickName => To grant operator status to a user." << std::endl;
//         Utils::print(G, "Response  :server.example.com 324 yourNickName #channel +o userNickName");
//         if (!ch->modifyOpsPrivileges(ch->getName(), target->getNickName(), '+')) {
//             Utils::print(R, ":server.example.com 441 yourNickName userNickName #channel :They aren't on that channel.");
//             return (ERR_USERNOTINCHANNEL);
//         }
//     }
//     else if (mode[0] == '-') {
//         std::cout << "MODE #channel -o userNickName => To remove operator status from a user." << std::endl;
//         Utils::print(G, "Response  :server.example.com 324 yourNickName #channel -o userNickName");
//         if (!ch->modifyOpsPrivileges(ch->getName(), target->getNickName(), '-')) {
//             Utils::print(R, ":server.example.com 441 yourNickName userNickName #channel :They aren't on that channel.");
//             return (ERR_USERNOTINCHANNEL);
//         }
//     }
//     else {
//         std::cout << "Invalid mode 'o' command." << std::endl;
//         Utils::print(R, ":server.example.com 472 yourNickName #channel :Unknown mode char");
//         return (ERR_UNKNOWNMODE);
//     }
//     return (RPL_CHANNELMODEIS);
// }

// int handleTopicMode(Request req, Channel *ch) {

//     (void) ch;
//     std::string mode = req.getParams()[1];
//     if (req.getParams().size() != 2) {
//         std::cout << "Invalid mode 't' command." << std::endl;
//         Utils::print(R, "Response  :server.example.com 461 yourNickName MODE :Not enough parameters");
//         return (NOT_ENOUGH_PARAMS);
//     }
//     if (mode[0] == '+') {
//         std::cout << "/MODE #channel +t => To set the +t mode to restrict chanops from changing the topic." << std::endl;
//         Utils::print(G, "Response  :server.example.com 324 yourNickName #channel +t");
//         ch->setTopicMode(off);
//     }
//     else if (mode[0] == '-') {
//         std::cout << "/MODE #channel -t =>To remove the +t mode and allow chanops to change the topic." << std::endl;
//         Utils::print(G, "Response  :server.example.com 324 yourNickName #channel -t");
//         ch->setTopicMode(on);
//     }
//     else {
//         std::cout << "Invalid mode 't' command." << std::endl;
//         Utils::print(R, ":server.example.com 472 yourNickName #channel :Unknown mode char");
//         return (ERR_UNKNOWNMODE);
//     }
//     return (RPL_CHANNELMODEIS);
// }

// int handleInviteOnlyMode(Request req, Channel *ch) {
//     std::string mode = req.getParams()[1];
//     if (req.getParams().size() != 2) {
//         std::cout << "Invalid mode 't' command." << std::endl;
//         Utils::print(R, "Response  :server.example.com 461 yourNickName MODE :Not enough parameters");
//         return (NOT_ENOUGH_PARAMS);
//     }
//     if (mode[0] == '+') {
//         std::cout << "/MODE #channel +i => This command sets the channel #channel to be invite-only." << std::endl;
//         Utils::print(G, "Response   :server.example.com 329 yourNickName #channel +i");
//         ch->setInviteOnlyMode(on);
//     }
//     else if (mode[0] == '-') {
//         std::cout << "/MODE #channel -i => This command removes the invite-only restriction from the channel #channel." << std::endl;
//         Utils::print(G, "Response   :server.example.com 329 yourNickName #channel -i");
//         ch->setInviteOnlyMode(off);
//     }
//     else {
//         std::cout << "Invalid mode 'i' command." << std::endl;  
//         Utils::print(R, ":server.example.com 472 yourNickName #channel :Unknown mode char");
//         return (ERR_UNKNOWNMODE);
//     }
//     return (RPL_CHANNELMODEIS);
// }

// int handleUserLimitMode(Request req, Channel *ch) {
//     std::string mode = req.getParams()[1];
//     if (req.getParams().size() == 3) {
//         if (mode[0] == '+') {
//             std::cout << "/MODE #channel +l x => This command sets the channel #channel with a user limit of x given." << std::endl;
//             if (!isValidUnsignedInt(req.getParams()[2])) {
//                 Utils::print(R, ":server.example.com 461 yourNickName MODE :Not enough parameters");
//                 return (printf("not valid limit\n"), NOT_ENOUGH_PARAMS);
//             }
//             ch->setUserLimitMode(on);
//             unsigned int limit = static_cast<unsigned int>(std::atoi(req.getParams()[2].c_str()));
//             if (!ch->setUserLimit(limit)) {
//                 Utils::print(R, ":server.example.com 461 yourNickName MODE :Not enough parameters");
//                 return (printf("%d is not valid limit\n", limit), NOT_ENOUGH_PARAMS);
//             }
//             Utils::print(G, "User limit set to " + std::to_string(limit) + ".");
//             Utils::print(G, "Response    :server.example.com 324 yourNickName #channel +l GIVEN_LIMIT");

//         }
//         else {
//             std::cout << "Invalid mode 'l' command." << std::endl;
//             Utils::print(R, ":server.example.com 461 yourNickName MODE :Not enough parameters");
//             return (NOT_ENOUGH_PARAMS);
//         }
//     }
//     else {
//         if (req.getParams().size() != 2) {
//             std::cout << "Invalid mode 'l' command." << std::endl;
//             Utils::print(R, "Response  :server.example.com 461 yourNickName MODE :Not enough parameters");
//             return (NOT_ENOUGH_PARAMS);
//         }
//         if (mode[0] == '-') {
//             std::cout << "/MODE #channel -l => This command removes the user limit from the channel #channel." << std::endl;
//             Utils::print(G, "Response   :server.example.com 329 yourNickName #channel -l");
//             ch->setUserLimitMode(off);
//         }
//         else {
//             std::cout << "Invalid mode 'l' command." << std::endl;
//             Utils::print(R, ":server.example.com 461 yourNickName MODE :Not enough parameter");
//             return (NOT_ENOUGH_PARAMS);
//         }
//     }
//     return (RPL_CHANNELMODEIS);
// }

// int handleKeyMode(Request req, Channel *ch) {
//     std::string mode = req.getParams()[1];
//     if (req.getParams().size() == 3) {
//         if (mode[0] == '+') {
//             std::cout << "/MODE #channel +k YourPassword => This command sets the channel #channel with the key 'YourPassword.'" << std::endl;
//             Utils::print(G, "Response   :server.example.com 329 yourNickName #channel YourPassword");
//             ch->setPasswordMode(on);
//             ch->setPassword(req.getParams()[2]);
//         }
//         else {
//             std::cout << "Invalid mode 'k' command." << std::endl;
//             Utils::print(R, ":server.example.com 461 yourNickName MODE :Not enough parameters.");
//             return (NOT_ENOUGH_PARAMS);
//         }
//     }
//     else {
//         if (req.getParams().size() != 2) {
//             std::cout << "Invalid mode 't' command." << std::endl;
//             Utils::print(R, "Response  :server.example.com 461 yourNickName MODE :Not enough parameters");
//             return (NOT_ENOUGH_PARAMS);
//         }
//         if (mode[0] == '-') {
//             std::cout << "/MODE #channel -k => This command removes the channel key from the channel #channel." << std::endl;
//             Utils::print(G, "Response   :server.example.com 329 yourNickName #channel -k");
//             ch->setPasswordMode(off);
//         }
//         else {
//             std::cout << "Invalid mode 'k' command." << std::endl;
//             Utils::print(R, "Response   :server.example.com 461 yourNickName MODE :Not enough parameter");
//             return (NOT_ENOUGH_PARAMS);
//         }
//     }
//     return (RPL_CHANNELMODEIS);
// }

// int handleChannelMode(int fd, Client *target, Request req, Channel *ch) {
//     (void) fd;

//     if (req.getParams().size() < 2) {
//         Utils::print(R, "Invalid mode command.");
//         Utils::print(R, "Response   :server.example.com 461 yourNickName MODE :Not enough parameters");
//         return (NOT_ENOUGH_PARAMS);
//     }
//     std::string mode = req.getParams()[1];
//     if (mode.length() != 2) {
//         Utils::print(R, "Response   :server.example.com 472 yourNickName #channel :Unknown mode char");
//         return (ERR_UNKNOWNMODE);
//     }
//     if (mode[0] != + '+' && mode[0] != '-') {
//         Utils::print(R, "Response   :server.example.com 472 yourNickName #channel :Unknown mode char");
//         return (ERR_UNKNOWNMODE);
//     }

//     mode = Utils::to_upper(mode);

//     switch (mode[1]) {
//         case 'O':
//             return handleOperatorMode(target, req, ch);
//         case 'K':
//             return handleKeyMode(req, ch);
//         case 'L':
//             return handleUserLimitMode(req, ch);
//         case 'T':
//             return handleTopicMode(req, ch);
//         case 'I':
//             return handleInviteOnlyMode(req, ch);
//         default:
//             Utils::print(R, "(In default) Response   :server.example.com 472 yourNickName #channel :Unknown mode char");    //  (ERR_UNKNOWNMODE);
//     }
//     return (RPL_CHANNELMODEIS);
// }

// int channel_playground(void) {


//     Client client1(1);  // Assuming file descriptor 1
//     client1.setNickName("client1");
//     Client client2(2);  // Assuming file descriptor 2
//     client2.setNickName("client2");
//     Client client3(3);  // Assuming file descriptor 3
//     client3.setNickName("client3");

//     if (general.join(&client1, 1)) {
//         std::cout << "Client in " + general.getName() + " !" << std::endl;
//     }
//     if (general.join(&client2, 2)) {
//         std::cout << "Client in " + general.getName() + " !" << std::endl;
//     }

//     // general.setUserLimitMode(on);
//     // general.setUserLimit(2);

//     if (general.join(&client3, 3)) {
//         std::cout << "Client in " + general.getName() + " !" << std::endl;
//     }

//     // general.getMembers();

//     // general.setTopicMode(on);
//     // general.setTopic("Jurassic Age");

//     // general.setPasswordMode(on);
//     // general.setPassword("123456");

//     // general.setInviteOnlyMode(on);

//     // general.setUserLimit(120);

//     Utils::print(Y, "Topic: |" + general.getTopic() + "|");
//     Utils::print(G, "Password: |" + general.getPassword() + "|");
//     Utils::print(P, "User Limit: ", 0);
//     std::cout << general.getLimit() << std::endl;
//     Utils::print(G, "Invite Only: ", 0);
//     std::cout << general.isInviteOnly() << std::endl;


//     std::cout << std::endl;
//     std::cout << std::endl;
//     std::cout << std::endl;

//     Utils::print(B, "<------- Creating Requests ------->");
//     Request req1;
//     req1.setCommand("MODE");
//     req1.setParams("#channel");
//     req1.setParams("+k");
//     // req1.setParams("23");
//     // req1.setParams("client2");

//     if (userIsChannelOp(&client1, &general)) {
//         handleChannelMode(1, &client2, req1, &general);
//     }
//     else {
//         Utils::print(R, ":server.example.com 461 yourNickName MODE :You're not a channel operator");
//     }


//     Utils::print(Y, "Topic: |" + general.getTopic() + "|");
//     Utils::print(G, "Password: |" + general.getPassword() + "|");
//     Utils::print(P, "User Limit: ", 0);
//     std::cout << general.getLimit() << std::endl;
//     Utils::print(G, "Invite Only: ", 0);
//     std::cout << general.isInviteOnly() << std::endl;
//     return 0;
// }

int main(int argc, char **argv) {

    // channel_playground();
    // return 0;
    if (argc != 3) return (Utils::print_error(INVALID_ARGC));
    Server server(SERVER_NAME, SERVER_VERSION, argv[1], argv[2]);
    if (server.valid_args() || server.start_server()) return (1);
    server.run();
};