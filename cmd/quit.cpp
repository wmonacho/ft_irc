#include "cmd.hpp"

bool	cmd::parseQuit(std::string str, User * user)
{
	std::vector<std::string> arg = splitString(str, " ");
	if (arg.size() != 2) {
		return false;
	}
	std::string quit_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + "\r\n";
	send(user->getSocket(), quit_message.c_str(), quit_message.size(), 0);
	std::cerr << "QUIT COMMAND" << std::endl;
	return true;
}