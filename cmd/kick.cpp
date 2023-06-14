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
    if (arg.size() < 3)
    {
        std::cerr << "Error: need more params" << std::endl;
        return false;
    }
     if (arg[index_channel].find("#") == std::string::npos && arg[index_channel].find("&") == std::string::npos)
     {
	 	std::cerr << arg[index_channel] << std::endl;
         std::cerr << "Error: bad channel mask" << std::endl;
         return false;
     }
     if (!server->channelAlreadyExist(&arg[index_channel][1]))
     {
         std::cerr << "Error: no such channel" << std::endl;
         return false;
     }
     if (!server->userInChannel(&arg[index_channel][1], server->getChannelUser(&arg[index_channel][1], arg[2])))
	 {
	 	std::cerr << "Error: not on channel" << std::endl;
	 	return false;
	 }
	
    // Cas 1 : il y a un commentaire au KICK
	std::cout << "channel ==" << &arg[index_channel][1] << " === user ==" << arg[2] << std::endl;
	std::cout << "befor kick :" << server->getChannelUser(&arg[index_channel][1], server->getUser(arg[2]))->getNickname() << std::endl;
	server->kickUserFromChannel(&arg[index_channel][1], server->getUser(arg[2]));
	if (server->getChannelUserList(&arg[index_channel][1]).find(server->getUser(arg[2])) == server->getChannelUserList(&arg[index_channel][1]).end())
		std::cout << "after kick : SUCCES" << std::endl;
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
 