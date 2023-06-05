#include "cmd.hpp"

bool    cmd::parseKick(std::string str, Server *server, User *user)
{
    std::cout << "Kick cmd found" << std::endl;
	std::cout << str << std::endl;
    std::vector<std::string> arg = splitString(str, " ");

    //check si au moins 3 param : need more params
    if (arg.size() < 3)
    {
        std::cerr << "Error: need more params" << std::endl;
        return false;
    }
    //check si # ou & devant le server
    if (arg[2][0] != '#' && arg[2][0] != '&')
    {
		std::cerr << arg[2] << std::endl;
        std::cerr << "Error: bad channel mask" << std::endl;
        return false;
    }
    //check si le server existe
    // if (!server->channelAlreadyExist(&arg[2][1]))
    // {
    //     std::cerr << "Error: no such channel" << std::endl;
    //     return false;
    // }
    // //check si le User exist dans le channel
    // if (!server->userInChannel(&arg[2][1], server->getChannelUser(&arg[2][1], arg[2])))
	// {
	// 	std::cerr << "Error: not on channel" << std::endl;
	// 	return false;
	// }
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
	arg[3].erase(0, 1);
	server->kickUserFromChannel(&arg[2][1], server->getChannelUser(&arg[2][1], arg[3]));
	//ecris la phrase de kick par default
	std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[2] + " " + arg[3] + "\r\n";
	std::cout << kick_message << std::endl;
	send(user->getSocket(), kick_message.c_str(), kick_message.size(), 0);
    return true;
}
