#include "cmd.hpp"

bool	cmd::parseKick(std::string str, Server *server, User *user)
{
	int	channelArgID;
	int	userToKickArgID;
	int	firstCharOfName;
	int	messageId = 0;
	bool	kickInChannel = false;
	int		channelArgumentNumber = 0;

	std::vector<std::string> arg = splitString(str, " ");
	if (arg.size() < 3)
	{
		// 461 ERR_NEEDMOREPARAMS
		std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + arg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	for (unsigned long i = 0; i < arg.size(); i++) {
		if (arg[i][0] == '#')
			channelArgumentNumber++;
		if (arg[i][0] == ':')
			messageId = i;
	}

	// arg[1] is always the channel if we kick from the channel. If we kick from the server arg[1] is hostname
	if (arg[1][0] == '#') {
		kickInChannel = true;
		if (channelArgumentNumber == 1) {
			// We kicked from a channel but with the light command : /kick user (comment)
			channelArgID = 1;
			userToKickArgID = 2;
			firstCharOfName = 0;
		}
		else {
			// We kicked from a channel but with the full command : /kick #channel user (comment)
			channelArgID = 2;
			userToKickArgID = 3;
			firstCharOfName = 1;
			messageId++;
		}
	} else {
		channelArgID = 2;
		userToKickArgID = 3;
		firstCharOfName = 1;
	}

	if (arg[channelArgID][0] != '#' && arg[channelArgID][0] != '&')
	{
		// 476 ERR_BADMASKCHANNEL
		std::string error = std::string(":localhost ") + "476 " + user->getNickname() + " " + arg[channelArgID] + " :Bad Channel Mask" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	if (!server->channelAlreadyExist(arg[channelArgID]))
	{
		// 403 ERR_NOSUCHCHANNEL
		std::string error = std::string(":localhost ") + "403 " + user->getNickname() +  " :No such channel" + "\r\n";
		return false;
	}

	if (!server->nickAlreadyExist(&arg[userToKickArgID][firstCharOfName]))
	{
		// 441 ERR_USERNOTINCHANNEL
		std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + arg[channelArgID] + " :They aren't on that channel" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	if (!server->userInChannel(arg[channelArgID], server->getChannelUser(arg[channelArgID], &arg[userToKickArgID][firstCharOfName])))
	{
		// 441 ERR_USERNOTINCHANNEL
		std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + arg[channelArgID] + " :They aren't on that channel" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	if (!server->getChannelUserAdmin(arg[channelArgID], user)) {
		// 482    ERR_CHANOPRIVSNEEDED
        std::string error = std::string(":localhost ") + "482 " + user->getNickname() + " " +  arg[channelArgID] + " :You're not channel operator" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return (false);
	}
	
	// Cas 1 : On KICK dans un channel
	if (kickInChannel == true && arg.size() >= 4) {
		
		rebuildMessage(arg, messageId);
		std::string kick_comment = arg[messageId];

		server->kickUserFromChannel(arg[channelArgID], server->getUser(&arg[userToKickArgID][firstCharOfName]));
		std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " " + kick_comment + "\r\n";
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(arg[channelArgID]));
		std::string kick_message_solo = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " " + kick_comment + "\r\n";
		send(server->getUser(&arg[userToKickArgID][firstCharOfName])->getSocket(), kick_message_solo.c_str(), kick_message_solo.size(), 0);
		
		if (server->getChannelUserList(arg[channelArgID]).size() != 0 && !server->channelHasOperator(arg[channelArgID])) {
			server->setChannelRemplacementOpe(arg[channelArgID]);
			std::string rpl_channel_mode_is = ":localhost MODE " + arg[channelArgID] + " +o " + server->getChannelUserList(arg[channelArgID]).begin()->first->getNickname() + "\r\n";
			this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, server->getChannel(arg[channelArgID]));
		}
	
		if (server->getChannelUserList(arg[channelArgID]).size() == 0)
			server->deleteChannel(arg[channelArgID]);
		return true;
	}

	if (kickInChannel == false && arg.size() > 4)
	{
		rebuildMessage(arg, 4);
		std::string kick_comment = arg[4];
		server->kickUserFromChannel(arg[channelArgID], server->getUser(&arg[userToKickArgID][firstCharOfName]));
		std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " " + kick_comment + "\r\n";
		sendMessageToAllUsersInChannel(kick_message, server->getChannel(arg[channelArgID]));
		std::string kick_message_solo = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " " + kick_comment + "\r\n";
		send(server->getUser(&arg[userToKickArgID][firstCharOfName])->getSocket(), kick_message_solo.c_str(), kick_message_solo.size(), 0);
		std::cerr << "userlist :" << server->getChannelUserList(arg[channelArgID]).size() << "  channel ope :" << server->channelHasOperator(arg[channelArgID]) << std::endl;

		if (server->getChannelUserList(arg[channelArgID]).size() != 0 && !server->channelHasOperator(arg[channelArgID])) {
			server->setChannelRemplacementOpe(arg[channelArgID]);
			std::string rpl_channel_mode_is = ":localhost MODE " + arg[channelArgID] + " +o " + server->getChannelUserList(arg[channelArgID]).begin()->first->getNickname() + "\r\n";
			this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, server->getChannel(arg[channelArgID]));
		}
		if (server->getChannelUserList(arg[channelArgID]).size() == 0) {
			server->deleteChannel(arg[channelArgID]);
		}
		return true;
	}

	// Cas 2 : pas de commentaire au KICK
	server->kickUserFromChannel(arg[channelArgID], server->getUser(&arg[userToKickArgID][firstCharOfName]));
	std::string kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " :You are KICK man" + "\r\n";
	sendMessageToAllUsersInChannel(kick_message, server->getChannel(arg[channelArgID]));
	kick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[channelArgID] + " " + &arg[userToKickArgID][firstCharOfName] + " :You are KICK man" + "\r\n";
	send(server->getUser(&arg[userToKickArgID][firstCharOfName])->getSocket(), kick_message.c_str(), kick_message.size(), 0);
	
	if (server->getChannelUserList(arg[channelArgID]).size() != 0 && !server->channelHasOperator(arg[channelArgID])) {
		server->setChannelRemplacementOpe(arg[channelArgID]);
		std::string rpl_channel_mode_is = ":localhost MODE " + arg[channelArgID] + " +o " + server->getChannelUserList(arg[channelArgID]).begin()->first->getNickname() + "\r\n";
		this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, server->getChannel(arg[channelArgID]));
	}
	
	if (server->getChannelUserList(arg[channelArgID]).size() == 0)
			server->deleteChannel(arg[channelArgID]);
	return true;
}
 