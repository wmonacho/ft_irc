#include "cmd.hpp"

bool    cmd::parsePart(std::string str, Server *server, User *user)
{
	(void)server;
	// Parsing de l'input du user
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() < 2)
    {
        //numeric reply
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return (false);
    }

	// On verifie que les channels renseignes dans la commande existent bien et que l'utilisateur est dedans
	// et on envoie un PART_reply pour chaque channel a quitter
    std::vector<std::string> channels = splitString(splitArg[1], ",");
    std::vector<std::string>::iterator it = channels.begin();
	while (it != channels.end())
    {
		// //verifier si le channel existe
		// if (!server->channelAlreadyExist(*it)) {
		// 	std::cerr << "FIRST FALSE" << std::endl;
		//     return false;
		// }
    	// //verifier si l'user est bien dans le channel
		// if (!server->userInChannel(*it, user)) {
		// 	std::cerr << "FIRST FALSE" << std::endl;
		//     return false;
		// }
		std::string part_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + splitArg[0] + " ";
		part_message.append(*it);
		part_message.append("\r\n");
		send(user->getSocket(), part_message.c_str(), part_message.size(), 0);
		it++;
    }
    return true;
}