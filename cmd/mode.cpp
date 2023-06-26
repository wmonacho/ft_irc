#include "cmd.hpp"

bool	cmd::parseMode(std::string str, Server *server, User *user)
{
	//mode i, t, k, o, l
	std::vector<std::string> splitArg = splitString(str, " ");
	//MODE <cible> <mode> <argument(s)>

	// Reponse au client apres le join d'un user pour lui informer des modes active dans ce channel
	if (splitArg.size() == 2)
	{
		// MODE <channel>
		//si le channel existe, renvoyer les modes actives pour celui ci
		if (server->channelAlreadyExist(splitArg[1])) {

			std::string rpl_channel_mode_is = std::string(":localhost ") + "467 " + user->getNickname() + " " + splitArg[1] + " +";
			if (server->getChannel(splitArg[1])->getInviteOnly() == true)
				rpl_channel_mode_is += "i";
			if (server->getChannel(splitArg[1])->getTopicAdmin() == true)
				rpl_channel_mode_is += "t";
			if (server->getChannel(splitArg[1])->getPassword() != "")
				rpl_channel_mode_is += "k";
			if (server->getChannel(splitArg[1])->getUserAdmin(user) == true)
				rpl_channel_mode_is += "o";
			if (server->getChannel(splitArg[1])->getUserLimit() != -1)
				rpl_channel_mode_is += "l";
			rpl_channel_mode_is += "\r\n";
			send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
			return (true);
		}
	}

	if (splitArg.size() < 3)
	{
		// 461  ERR_NEEDMOREPARAMS
		std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return (false);
	}

	//check si # devant la cible et si le channel existe
	if ((splitArg[1][0] != '#' && splitArg[1][0] != '&') || server->getMap().find(splitArg[1]) == server->getMap().end())
	{
		std::string error = std::string("localhost :") + "476 " + user->getNickname() + " " + splitArg[1] + " :Bad Channel Mask" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return (false);
	}

	//check si le User est bien dans la userlist du channel
	if (!server->userInChannel(splitArg[1], user))
	{
			// 441 ERR_USERNOTINCHANNEL
			std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + splitArg[1] + " :They aren't on that channel" + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
			return false;
	}

	//check si + ou - devant le mode
	if ((splitArg[2][0] != '-' && splitArg[2][0] != '+'))
	{
			// 477 ERR_NOCHANMODES
			std::string error = std::string(":localhost ") + "477 " + user->getNickname() + " " +  splitArg[1] + " :Channel doesn't support modes" + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
			return (false);
	}

	//check si le mode existe (tout depend de ceux que l'on prend)
	std::string modes = "iktlo";	
	for (unsigned int i = 1; i < splitArg[2].size() ; i++)
	{
			if ( modes.find(splitArg[2][i]) == std::string::npos) {
				// 472 ERR_UNKNOWNMODE
				std::string error = std::string(":localhost ") + "472 " + user->getNickname() + " " +  splitArg[2][i] + " :is unknown mode char to me for " + splitArg[1] + "\r\n";
				send(user->getSocket(), error.c_str(), error.size(), 0);
				return (false);
			}
	}

	// Check userAdmin ::
	if (!server->getChannelUserAdmin(splitArg[1], user)) {
		// 482    ERR_CHANOPRIVSNEEDED
        std::string error = std::string(":localhost ") + "482 " + user->getNickname() + " " +  splitArg[1] + " :You're not channel operator" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return (false);
	}
	//execute les modes +
	//il restera a modifier les fonctions affectees par les modes
	Channel *chan = server->getChannel(splitArg[1]);

	for (unsigned int i = 1; splitArg[2][0] == '+' && i < splitArg[2].size(); i++)
	{	
		std::string rpl_channel_mode_is = ":localhost MODE ";
		switch(splitArg[2][i])
		{
				  case 105:
						//execute mode i
						chan->setInviteOnly(true);
						// 324 RPL_CHANNELMODEIS
						//send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
						rpl_channel_mode_is += chan->getName() + " +i" + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
						break;
				  case 107:
						//execute mode k
						if (chan->getPassword() != "") {
							// 467 ERR_KEYSET
							std::string error = std::string(":localhost ") + "467 " + user->getNickname() + " " + splitArg[1] + " :Channel key already set" + "\r\n";
							send(user->getSocket(), error.c_str(), error.size(), 0);
							return (false);
						}
						if (i + 2 < splitArg.size()) {
							chan->setPassword(splitArg[i + 2]);
							// 324 RPL_CHANNELMODEIS
							rpl_channel_mode_is += chan->getName() + " +k " + splitArg[i + 2] + "\r\n";
							this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
						}
						else
						{
							// 461  ERR_NEEDMOREPARAMS
							std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
							send(user->getSocket(), error.c_str(), error.size(), 0);
							return (false);
						}
						break;
				  case 108:
					  	//execute mode L
						chan->setUserLimit(atoi(splitArg[i + 2].c_str()));
						rpl_channel_mode_is += chan->getName() + " +l " + splitArg[i + 2] + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
						break;
				  case 111:
					  	//execute mode o
						if (i + 2 < splitArg.size()) {
							rpl_channel_mode_is += chan->getName() + " +o " + splitArg[i + 2] + "\r\n";
							chan->changeUserAdmin(server->getChannelUser(splitArg[1], splitArg[i + 2]), true);
							this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
							server->sendUserList(chan, user);
						}
						else
						{
							// 461  ERR_NEEDMOREPARAMS
							std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
							send(user->getSocket(), error.c_str(), error.size(), 0);
							return (false);
						}
					  	break;
				  case 116:
					  	//execute mode t
					  	chan->setTopicAdmin(true);
						rpl_channel_mode_is += chan->getName() + " +t" + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
					  	break;
		}
	}
	
	//execute les modes -
	for (unsigned int i = 1; splitArg[2][0] == '-' && i < splitArg[2].size(); i++)
	{
		std::string rpl_channel_mode_is = ":localhost MODE ";
		switch(splitArg[2][i])
		{
				  case 105:
					  	//execute mode i
					  	chan->setInviteOnly(false);
					  	// 324 RPL_CHANNELMODEIS
						rpl_channel_mode_is += chan->getName() + " -i" + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
					  break;
				  case 107:
					  	//execute mode k
					  	chan->setPassword("");
					  	// 324 RPL_CHANNELMODEIS
						rpl_channel_mode_is += chan->getName() + " -k" + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
					  break;
				  case 108:
					  	//execute mode L
					  	chan->setUserLimit(-1);
					  	// 324 RPL_CHANNELMODEIS
						rpl_channel_mode_is += chan->getName() + " -l" + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
					  break;
				  case 111:
					  	//execute mode o
						if (i + 2 < splitArg.size()) {
					  		chan->changeUserAdmin(server->getChannelUser(splitArg[1], splitArg[i + 2]), false);
					  		// 324 RPL_CHANNELMODEIS
							rpl_channel_mode_is += chan->getName() + " -o " + splitArg[i + 2] + "\r\n";
							this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
							server->sendUserList(chan, user);
						}
						else
						{
							// 461  ERR_NEEDMOREPARAMS
							std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
							send(user->getSocket(), error.c_str(), error.size(), 0);
							return (false);
						}
					  break;
				  case 116:
					  	chan->setTopicAdmin(false);
					  	// 324 RPL_CHANNELMODEIS
						rpl_channel_mode_is += chan->getName() + " -t" + "\r\n";
						this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, chan);
					  break;
		}
	}
	return (true);
}

// Numeric Replies:

//
//                           ERR_CHANOPRIVSNEEDED a voir
//                       
//          346 RPL_INVITELIST              347    RPL_ENDOFINVITELIST
//           RPL_UNIQOPIS
