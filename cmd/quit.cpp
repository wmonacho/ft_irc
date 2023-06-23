#include "cmd.hpp"

bool	cmd::parseQuit(Server *server, std::string str, User * user)
{
	std::vector<std::string> arg = splitString(str, " ");
	if (arg.size() != 2) {
		return false;
	}

	//delete all chan
	if (server->getUserList().size() == 1) {
		server->deleteAllChannel();
		std::string quit_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + "\r\n";
		send(user->getSocket(), quit_message.c_str(), quit_message.size(), 0);
	}

	// crrer une methode ->
	server->deleteUserFromUserList(*user);

	std::cerr << "QUIT COMMAND" << std::endl;
	return true;
}