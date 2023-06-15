#include "cmd.hpp"

bool    cmd::parseKick(std::string str, Server *server, User *user)
{
	(void)server;
	int	index_channel = 1;

	// Parsing de l'input du user
	// faire un check de channel si en arg[1] ou si en arg[2] si aucun return false
    std::vector<std::string> arg = splitString(str, " ");
	if (server->channelAlreadyExist(&arg[2][1]))
		index_channel = 2;
	std::cout << "channel ===" << &arg[index_channel][1] << std::endl;
    if (arg.size() < 3)
    {
        // 461 ERR_NEEDMOREPARAMS
        std::string error = std::string("localhost :") + "461 " + user->getNickname() + " " + arg[0] + " :Not enough parameters" + "\r\n";
        send(user->getSocket(), error.c_str(), error.size(), 0);
	    return false;
    }
    if (arg[index_channel].find("#") == std::string::npos && arg[index_channel].find("&") == std::string::npos)
    {
		// 476 ERR_BADMASKCHANNEL
		std::string error = std::string("localhost :") + "476 " + user->getNickname() + " " + arg[index_channel] + " :Bad Channel Mask" + "\r\n";
        send(user->getSocket(), error.c_str(), error.size(), 0);
	    return false;
    }
    if (!server->channelAlreadyExist(&arg[index_channel][1]))
    {
		// 403 ERR_NOSUCHCHANNEL
        std::string error = std::string("localhost :") + "403 " + user->getNickname() +  " :No such channel" + "\r\n";
        return false;
    }
    if (!server->userInChannel(&arg[index_channel][1], server->getChannelUser(&arg[index_channel][1], arg[2])))
	{
		// 441 ERR_USERNOTINCHANNEL
		std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + arg[index_channel] + " :They aren't on that channel" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
        return false;
	}
	
    // Cas 1 : il y a un commentaire au KICK
	server->kickUserFromChannel(&arg[index_channel][1], server->getUser(arg[2]));
	if (arg.size() == 5)
	{
		if (arg[4][0] != ':')
		{
			std::cerr << "Error: wrong parameters" << std::endl;
			return false;
		}
		rebuildMessage(arg, 3);
		std::string kick_comment = arg[4];
		std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + kick_comment + "\r\n";
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(&arg[index_channel][1]));
		return true;
	}

	// Cas 2 : pas de commentaire au KICK
	std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + " You are KICK man" + "\r\n";
	std::cout << kick_message << std::endl;
	sendMessageToAllUsersInChannel(kick_message, server->getChannel(&arg[index_channel][1]));
    return true;
}
/*---------------------------------------------*/
				/* HELP */
// arg[2] = channel
// arg[3] = user
// arg[4] = message

// KICK channel user :message --> size = 5
// KICK channel user --> size = 4
/*---------------------------------------------*/
 