#include "../incl/Server.hpp"
#include "../incl/Utils.hpp"
#include "../incl/MessageParser.hpp"

#define SERVER_NAME "MR.SERVER.com"

int main(int argc, char **argv) {

	Request clientRequest = MessageParser::parseMsg(":sender!user@host PRIVMSG #channel :Hello, this is a message!");

	std::cout << std::endl;
	std::cout << "Prefix: " << clientRequest.getPrefix() << std::endl;
	std::cout << "Command: " << clientRequest.getCommand() << std::endl;

	std::vector<std::string> tmpParams = clientRequest.getParams();
	std::vector<std::string>::iterator it = tmpParams.begin();
	std::vector<std::string>::iterator end = tmpParams.end();

	while (it != end) {
		std::cout << "Param: " << *it << std::endl;
		++it;
	}
	// for (size_t i = 0; i < clientRequest.getParams().size(); ++i) {
	// 	std::cout << "Param " << i << ": " << clientRequest.getParams()[i] << std::endl;
	// }
	std::cout << "Trailing: " << clientRequest.getTrailing() << std::endl;


    if (argc != 3) return (Utils::error(INVALID_ARGC));
    Server server(SERVER_NAME, argv[1], argv[2]);
    if (server.valid_args() || server.start_server()) return (1);
    server.run();
};