#include "cmd.hpp"

bool	cmd::parseQuit(Server *server, std::string str, User * user)
{
	std::vector<std::string> arg = splitString(str, " ");
	if (arg.size() != 2) {
		return false;
	}

	if (user)
		std::cerr << "The user [" << user->getNickname() << "] is leaving the server, he will be erased" << std::endl;
	this->parseJoin("JOIN 0", server, user);
	if (user) {
		server->deleteUserFromUserList(*user);
		std::cerr << "the user has been deleted from the database" << std::endl;
	}
	return true;
}