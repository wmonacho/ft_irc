#include "cmd.hpp"

bool    cmd::parsePrivmsg(std::string str, Server *server, User *user)
{
	// parsing de l'input user
	std::vector<std::string> arg = splitString(str, " ");
	std::string nick_target = arg[1];
	if (arg.size() > 3)
	{
		// attention car le message est pas rebuild donc a plusieurs argments
		// 407	ERR_TOOMANYTARGETS
		std::string error = generateErrorMessage("407", arg[0]);
		std::cout << error << std::endl;
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	if (arg[2].empty())
	{
		std::cerr << "ERROR 2" << std::endl;
		// 412	ERR_NOTEXTTOSEND
		std::string error = generateErrorMessage("412", arg[0]);
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	// if (!server->nickAlreadyExist(nick_target))
	// {
	// 	std::cerr << "ERR_NOSUCHNICK" << std::endl;
	// 	return false;
	// }

    // Cas 1 : le user parle dans un channel
    if (arg[1].find("#") != std::string::npos) {
        arg[1].erase(0, 1);
        if (server->channelAlreadyExist(arg[1])) {
            Channel *channel = server->getChannel(arg[1]);
            arg[1].insert(0, "#");
			rebuildMessage(arg, 2);
            std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
			sendMessageToOtherUsersInChannel(message, channel, user);
            return true ;
        }
		// 404	ERR_CANNOTSENDTOCHAN
		std::string error = generateErrorMessage("404", arg[0]);
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
    }

	// Cas 2 : le user parle directement a un autre user
    User *dest = server->getUser(nick_target);
	// 411 ERR_NORECIPIENT
	if (!dest) {
		std::string error = generateErrorMessage("411", arg[0]);
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	rebuildMessage(arg, 2);
    std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
    send(dest->getSocket(), message.c_str(), message.size(), 0);
	return true;
}