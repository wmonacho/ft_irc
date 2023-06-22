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


	std::vector<User*> user_list = server->getUserList();
	for (unsigned int i = 0; i < user_list.size(); i++) {
		if (user_list[i]->getNickname() == user->getNickname()) {
			std::vector<User*>::iterator it;
			it = user_list.begin();
			user_list.erase(it + i);
			break;
		}
	}

	for (unsigned int i = 0; i < user_list.size(); i++) {
		std::cerr << "after user :" << user_list[i]->getNickname() << std::endl;
	}

	std::cerr << "QUIT COMMAND" << std::endl;
	return true;
}