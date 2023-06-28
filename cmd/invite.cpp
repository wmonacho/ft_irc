#include "cmd.hpp"

bool	cmd::parseInvite(std::string str, Server *server, User *user)
{
	std::vector<std::string> arg = splitString(str, " ");

	if (arg.size() != 3)
		return false;

	int chanFound = 0;
	std::string nick = arg[1];
	std::string channel = arg[2];
	std::map<std::string, Channel*> map = server->getMap();

	for (std::map<std::string, Channel*>::iterator it = map.begin(); it != map.end(); it++)
	{
		if (it->first == channel)
		{
			chanFound = 1;
			break ;
		}
	}
	//channel pas valide = pas d'erreur
	if (chanFound == 0)
		return false;
	if (server->nickAlreadyExist(nick) == false)
	{
		// 401	ERR_NOSUCHNICK
		std::string error = std::string(":localhost ") + "401" + " " + user->getNickname() + " " + channel + " :No such nick/channel" + "\r\n";
		server->addReply(user->getSocket(), error);
		//send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	//seulement les users du channel peuvent inviter des gens
	//verifier que l'user soit bien dans le channel
	if (!server->userInChannel(channel, user))
	{
		// 442	ERR_NOTONCHANNEL
		std::string error = std::string(":localhost ") + "442" + " " + user->getNickname() + " " + channel + " :You're not on that channel" + "\r\n";
		server->addReply(user->getSocket(), error);
		//send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	//si le channel a le flag invite only set, seulement les channels operators peuvent inviter
	if (!server->getChannelUserAdmin(channel, user) && server->channelIsInviteOnly(channel))
	{
		// 482	ERR_CHANOPRIVSNEEDED
		std::string error = std::string(":localhost ") + "482" + " " + user->getNickname() + " " + channel + " :You're not channel operator" + "\r\n";
		server->addReply(user->getSocket(), error);
		//send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	//checker si l'user est deja sur le chan
	if (server->userInChannel(channel, server->getChannelUser(channel, nick)))
	{
		// 443	ERR_USERONCHANNEL
		std::string error = std::string(":localhost ") + "443" + " " + user->getNickname() + " " + nick + " " + channel + " :is already on channel" + "\r\n";
		server->addReply(user->getSocket(), error);
		//send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	//check si le channel a une limit
	if (server->channelHaveLimit(channel))
	{
		//check si la limit est inferieur ou egal aux users sur le channel
		if (!server->channelEnoughSpace(channel))
		{
			std::cerr << "ERR_BEYONDTHELIMIT" << std::endl;
			return false;
		}
	}

	// Execution de la cmd
	UserAspects channel_aspects(false);
	server->addUserToChannelInviteList(channel, nick);

	std::string invite_message = ":" + user->getNickname() + "!" + user->getNickname() + "@localhost " + arg[0] + " " + nick + " " + channel + "\r\n";
	std::string user_nickname = server->getUser(nick)->getNickname();
	std::string invite_confirmation = std::string(":localhost ") + "341" + " " + user->getNickname() + " " + user_nickname + " " + channel + "\r\n";
	send(user->getSocket(), invite_confirmation.c_str(), invite_confirmation.size(), 0);
	send(server->getUser(nick)->getSocket(), invite_message.c_str(), invite_message.size(), 0);
	return true;
}