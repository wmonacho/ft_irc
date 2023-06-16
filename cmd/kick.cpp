#include "cmd.hpp"

bool    cmd::parseKick(std::string str, Server *server, User *user)
{
	// Parsing de l'input du user
	// faire un check de channel si en arg[1] ou si en arg[2] si aucun return false
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() < 3)
    {
        // 461 ERR_NEEDMOREPARAMS
        std::string error = std::string("localhost :") + "461 " + user->getNickname() + " " + arg[0] + " :Not enough parameters" + "\r\n";
        send(user->getSocket(), error.c_str(), error.size(), 0);
	    return false;
    }
    if (arg[2][0] != '#' && arg[2][0] != '&')
    {
		// 476 ERR_BADMASKCHANNEL
		std::string error = std::string("localhost :") + "476 " + user->getNickname() + " " + arg[2] + " :Bad Channel Mask" + "\r\n";
        send(user->getSocket(), error.c_str(), error.size(), 0);
	    return false;
    }
    if (!server->channelAlreadyExist(&arg[2][1]))
    {
		// 403 ERR_NOSUCHCHANNEL
        std::string error = std::string("localhost :") + "403 " + user->getNickname() +  " :No such channel" + "\r\n";
        return false;
    }
    if (!server->userInChannel(&arg[2][1], server->getChannelUser(&arg[2][1], &arg[3][1])))
	{
		// 441 ERR_USERNOTINCHANNEL
		std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + arg[2] + " :They aren't on that channel" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
        return false;
	}
	
    // Cas 1 : il y a un commentaire au KICK
	server->kickUserFromChannel(&arg[2][1], server->getUser(&arg[3][1]));
	if (arg.size() > 4)
	{
		if (arg[4][0] != ':')
		{
			std::cerr << "Error: wrong parameters" << std::endl;
			return false;
		}
		rebuildMessage(arg, 3);
		std::string kick_comment;
		size_t i = 4;
		while (i < arg.size() - 1) {
			kick_comment += arg[i] + " ";
			i++;
		}
		kick_comment += arg[i];
		std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + kick_comment + "\r\n";
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(&arg[2][1]));
		return true;
	}

	// Cas 2 : pas de commentaire au KICK
	std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + " You are KICK man" + "\r\n";
	std::cout << kick_message << std::endl;
	sendMessageToAllUsersInChannel(kick_message, server->getChannel(&arg[2][1]));
	kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + " You are KICK man" + "\r\n";
	send(server->getUser(&arg[3][1])->getSocket(), kick_message.c_str(), kick_message.size(), 0);
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
 