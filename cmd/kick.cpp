#include "cmd.hpp"

bool    cmd::parseKick(std::string str, Server *server, User *user)
{
	(void)server;
	// Parsing de l'input du user
    std::vector<std::string> arg = splitString(str, " ");
	std::cout << "Size of ARG in KICK = " << arg.size() << std::endl;
	std::cout << "ARG[2] = " << arg[2] << std::endl;
    if (arg.size() < 3)
    {
        std::cerr << "Error: need more params" << std::endl;
        return false;
    }
<<<<<<< HEAD
    //check si # ou & devant le server
    if (arg[1][0] != '#' && arg[1][0] != '&')
    {
		std::cerr << arg[1] << std::endl;
        std::cerr << "Error: bad channel mask" << std::endl;
        return false;
    }
    //check si le channel existe
     if (!server->channelAlreadyExist(&arg[1][1]))
     {
         std::cerr << "Error: no such channel" << std::endl;
         return false;
     }
     //check si le User exist dans le channel
     if (!server->userInChannel(&arg[1][1], server->getChannelUser(&arg[1][1], arg[2])))
	 {
	 	std::cerr << "Error: not on channel" << std::endl;
	 	return false;
	 }
    //check si il y a un commentaire : si oui l'afficher
	// if (arg.size() > 3)
	// {
	// 	if (arg[3][0] != ':')
	// 	{
	// 		std::cerr << "Error: wrong parameters" << std::endl;
	// 		return false;
	// 	}
	// 	//bannir le User du channel
	//	server->kickUserFromChannel(&arg[1][1], server->getChannelUser(&arg[1][1], arg[2]));
	// 	//server->kickChannelUser(&arg[2][1], kick_user);
	// 	//ecris le commentaire de kick
	// 	unsigned int i = 3;
	// 	while (i < arg.size() - 1)
	// 	{
	// 		std::cout << arg[i] << " ";
	// 		i++;
	// 	}
	// 	std::cout << arg[i] << std::endl;
	// 	return true;
	// }
	//bannir le User du channel
	//arg[3].erase(0, 1);
	server->kickUserFromChannel(&arg[1][1], server->getChannelUser(&arg[1][1], arg[2]));
	//ecris la phrase de kick par default
=======
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
>>>>>>> 2f8209742287a45a7a0b26a7458466119b9e673a
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
 