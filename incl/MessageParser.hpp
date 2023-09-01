#pragma once
#include "./Shared.hpp"
#include "../incl/Request.hpp"
#include <sstream>


/* 
	* MessageParser is a class that parses the initial messages received from the server
*/
class MessageParser {
	private:
		MessageParser();
		~MessageParser();
	public:
		static Request parseMsg(std::string msg);
};