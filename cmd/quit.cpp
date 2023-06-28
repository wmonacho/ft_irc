#include "cmd.hpp"

bool	cmd::parseQuit(Server *server, std::string str, User * user)
{
	std::vector<std::string> arg = splitString(str, " ");
	if (arg.size() != 2) {
		return false;
	}

	this->parseJoin("JOIN 0", server, user);
	if (user) {
		server->deleteUserFromUserList(*user);
	}
	return true;
}