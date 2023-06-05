#include "cmd.hpp"

bool    cmd::parsePrivmsg(std::string str, Server *server, User *user)
{
	std::vector<std::string> arg = splitString(str, " ");
	std::string nick_target = arg[1];
	if (server->nickAlreadyExist(arg[2]) == true)
	{
		std::cerr << "ERR_TOOMANYTARGETS" << std::endl;
		return false;
	}
	if (arg[2][0] != ':')
	{
		std::cerr << "ERR_NOTEXTTOSEND" << std::endl;
		return false;
	}
	// if (!server->nickAlreadyExist(nick_target))
	// {
	// 	std::cerr << "ERR_NOSUCHNICK" << std::endl;
	// 	return false;
	// }

    // utiliser arg pour trouver le channel --> le recup et envoyer le message a tout les utilisateurs dedans
    if (arg[1].find("#") != std::string::npos) {
        std::cout << "ENTER CHANNEL USAGE" << std::endl;
        arg[1].erase(0, 1);
        std::cout << "CHANNEL ==> " << arg[1] << std::endl;
        if (server->channelAlreadyExist(arg[1])) {
            std::cout << "Channel exists so we can send to all users = " << arg[1] << std::endl; 
            Channel *channel = server->getChannel(arg[1]);
            arg[1].insert(0, "#");

            std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
			std::cout << "======> " << message << std::endl;
            sendResponseToAllUsersInChannel(message, channel);
            return true ;
        }
        std::cerr << "No channel found" << std::endl;
    }

    User *dest = server->getUser(nick_target);
    size_t i = 3;
	while (i < arg.size()) {
		arg[2].append(" ");
		arg[2].append(arg[i]);
		i++;
	}
	arg[2].append("\0");
    std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
    send(dest->getSocket(), message.c_str(), message.size(), 0);
	return true;
}