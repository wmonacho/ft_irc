#include "cmd.hpp"

bool    cmd::parseKick(std::string str, Server *server, User *user)
{
	(void)server;

	// Parsing de l'input du user
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() < 3)
    {
		std::string error = std::string(":localhost ") + "412" + " " + arg[0] + " :No text to send" + "\r\n";
		// std::cerr << "412	ERR_NOTEXTTOSEND --> " << message_check << " // " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
    }
    // if (arg[2].find("#") == std::string::npos || arg[2].find("&") == std::string::npos)
    // {
	// 	std::cerr << arg[2] << std::endl;
    //     std::cerr << "Error: bad channel mask" << std::endl;
    //     return false;
    // }
    // check si le server existe
    // if (!server->channelAlreadyExist(&arg[2][1]))
    // {
    //     std::cerr << "Error: no such channel" << std::endl;
    //     return false;
    // }
    // check si le User exist dans le channel
    // if (!server->userInChannel(&arg[2][1], server->getChannelUser(&arg[2][1], arg[2])))
	// {
	// 	std::cerr << "Error: not on channel" << std::endl;
	// 	return false;
	// }
	
    // Cas 1 : il y a un commentaire au KICK
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
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(&arg[2][1]));
		return true;
	}

	// Cas 2 : pas de commentaire au KICK
	std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + "\r\n";
	std::cout << kick_message << std::endl;
	sendMessageToAllUsersInChannel(kick_message, server->getChannel(&arg[2][1]));
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
 