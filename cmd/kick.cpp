#include "cmd.hpp"

bool	cmd::parseKick(std::string str, Server *server, User *user)
{
	int	channelArgID;
	int	userToKickArgID;
	int	firstCharOfName;
	bool	noChannelInCommand = false;

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

	if (arg[2][0] == '#')
		noChannelInCommand = true;

	// Cas X : un user KICK un autre user en etant dans un channel --> KICK #lol issou :mdr
	// dans ce cas la user[1][1] coupe le premier char
	// dans l'autre cas user[1][1] c'est bueno mdr
	if (arg.size() == 3) {
		channelArgID = 1;
		userToKickArgID = 2;
		firstCharOfName = 0;
	}
	else {
		channelArgID = 2;
		userToKickArgID = 3;
		firstCharOfName = 1;
	}


	if (arg[channelArgID][0] != '#' && arg[channelArgID][0] != '&')
	{
		// 476 ERR_BADMASKCHANNEL
		std::string error = std::string("localhost :") + "476 " + user->getNickname() + " " + arg[channelArgID] + " :Bad Channel Mask" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	if (!server->channelAlreadyExist(arg[channelArgID]))
	{
		// 403 ERR_NOSUCHCHANNEL
		std::string error = std::string("localhost :") + "403 " + user->getNickname() +  " :No such channel" + "\r\n";
		return false;
	}

	if (!server->userInChannel(arg[channelArgID], server->getChannelUser(arg[channelArgID], &arg[userToKickArgID][firstCharOfName])))
	{
		// 441 ERR_USERNOTINCHANNEL
		std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + arg[channelArgID] + " :They aren't on that channel" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	
	// Cas 1 : il y a un commentaire au KICK
	if (noChannelInCommand == true && arg.size() == 3) {
		
		// if (arg[3][0] != ':')
		// {
		// 	std::cerr << "Error: wrong parameters" << std::endl;
		// 	return false;
		// }
		rebuildMessage(arg, 3);
		std::string kick_comment = arg[3];

		server->kickUserFromChannel(arg[channelArgID], server->getUser(&arg[userToKickArgID][firstCharOfName]));
		std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][1] + " " + kick_comment + "\r\n";
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(arg[channelArgID]));
		std::string kick_message_solo = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " " + kick_comment + "\r\n";
		send(server->getUser(&arg[userToKickArgID][firstCharOfName])->getSocket(), kick_message_solo.c_str(), kick_message_solo.size(), 0);
		return true;
	}

	if (arg.size() > 4)
	{
		if (arg[4][0] != ':')
		{
			std::cerr << "Error: wrong parameters" << std::endl;
			return false;
		}
		rebuildMessage(arg, 4);
		std::string kick_comment = arg[4];

		server->kickUserFromChannel(arg[2], server->getUser(&arg[3][1]));
		std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[2] + " " + &arg[3][1] + " " + kick_comment + "\r\n";
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(arg[2]));
		std::string kick_message_solo = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[2] + " " + &arg[3][1] + " " + kick_comment + "\r\n";
		send(server->getUser(&arg[3][1])->getSocket(), kick_message_solo.c_str(), kick_message_solo.size(), 0);
		return true;
	}

	// Cas 2 : pas de commentaire au KICK
	std::cerr << "USER -----> " << &arg[userToKickArgID][firstCharOfName] << std::endl;
	server->kickUserFromChannel(arg[channelArgID], server->getUser(&arg[userToKickArgID][firstCharOfName]));
	std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " :You are KICK man" + "\r\n";
	std::cout << kick_message << std::endl;
	sendMessageToAllUsersInChannel(kick_message, server->getChannel(arg[channelArgID]));
	kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " :You are KICK man" + "\r\n";
	send(server->getUser(&arg[userToKickArgID][firstCharOfName])->getSocket(), kick_message.c_str(), kick_message.size(), 0);
	std::cerr << "DEBUUUUUUUUUUUUUUUUUUUUUG 1" << std::endl;
	return true;
}

/*---------------------------------------------*/
				/* HELP */
// arg[2] = channel
// arg[3] = user
// arg[4] = message

// KICK channel user :message --> size = 5
// KICK channel user --> size = 4

// KICK #lol issou --> dans un channel == bad channel mask
// KICK #lol #lol issou --> dans le serveur (quand on precise le channel)
/*---------------------------------------------*/
 