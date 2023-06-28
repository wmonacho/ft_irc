#include "cmd.hpp"

bool	cmd::parseJoin(std::string str, Server *server, User *user)
{
	// Parsing de la string (commande + argument )
	std::vector<std::string> splitArg = splitString(str, " ");


	if (splitArg.size() == 2 && splitArg[1] == "0") {
		server->partAllChannelWhereTheUserIsPresent(user);
		return true;
	}

	if (splitArg.size() < 2) {
		// 461	ERR_NEEDMOREPARAMS
		std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	std::vector<std::string> channels = splitString(splitArg[1], ",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i][0] != '#' && channels[i][0] != '&') {
			// 476	ERR_BADCHANNELMASK
			std::string error = std::string(":localhost ") + "476 " + user->getNickname() + " " + channels[i] + " :Bad Channel Mask" + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
			return false;
		}
	}

	std::string server_response = createServerMessage(user, "", splitArg);

	for (size_t i = 0; i < channels.size(); i++) {

		std::string channel_name = channels[i];
		std::string server_response = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + splitArg[0] + " " + channels[i] + "\r\n";
		
		// Cas 1 : le channel existe, donc y ajoute le user et on envoie le message de "bienvenue" a tout le monde
		// + on envoie le topic (s'il existe) et la liste des USER 
		if (server->channelAlreadyExist(channel_name)) {

			Channel* channel = server->getChannel(channel_name);
 
			if (channel->getInviteOnly() && !channel->userInInviteList(user->getNickname())) {
				// 473    ERR_INVITEONLYCHAN
			  	std::string error = std::string(":localhost ") + "473 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+i)" + "\r\n";
			  	send(user->getSocket(), error.c_str(), error.size(), 0);
			  	continue;
			}

			if (splitArg.size() < 3 && channel->getPassword() != "") {
				// 475	ERR_BADCHANNELKEY
				std::string error = std::string(":localhost ") + "475 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+k)" + "\r\n";
				send(user->getSocket(), error.c_str(), error.size(), 0);
				continue;
			}

			if (splitArg.size() > 2 && channel->getPassword() != "")
			{
				std::vector<std::string> passwords = splitString(splitArg[2], ",");
				if (passwords.size() < i) {
					//475    ERR_BADCHANNELKEY
					std::string error = std::string(":localhost ") + "475 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+k)" + "\r\n";
					send(user->getSocket(), error.c_str(), error.size(), 0);
					continue;
				}
				if (passwords[i] != channel->getPassword())
				{
					//475    ERR_BADCHANNELKEY
					std::string error = std::string(":localhost ") + "475 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+k)" + "\r\n";
					send(user->getSocket(), error.c_str(), error.size(), 0);
					continue;
				}
			}

			if (!server->channelEnoughSpace(channel->getName())) {
				// 471 ERR_CHANNELISFULL
				std::string error = std::string(":localhost ") + "471 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+l)" + "\r\n";
				send(user->getSocket(), error.c_str(), error.size(), 0);
				continue;
			}

 			UserAspects	new_aspects(false);

			server->addUserToChannel(channel_name, user, new_aspects);
			this->sendMessageToAllUsersInChannel(server_response, channel);

			// On envoie le topic s'il existe
			if (!channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "332 " + user->getNickname() + " " + channels[i] + " " + channel->getTopic() + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			if (channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "331 " + user->getNickname() + " " + channels[i] + " :Not topic is set" + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			// User list : we send a RPL_NAMREPLY
			server->sendUserList(channel, user);
			continue;
		}

		// Cas 2 : le channel n'existe pas, il faut donc le creer dans notre serveur et y ajouter l'utilisateur
		if (!server->channelAlreadyExist(channel_name)) {

		    	if (channel_name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 1) != std::string::npos)
			{
				 std::cerr << "Bad channel name: " << channel_name << " (Not only alphanum char)" << std::endl;
				 return false;
			}
			UserAspects	new_aspects(true);
			Channel *channel = new Channel(channel_name);

			server->createNewChannel(channel_name, channel);
			if (!server->getChannel(channel_name))
			{
				delete	channel;
				return false;
			}
			server->addUserToChannel(channel_name, user, new_aspects);
			sendMessageToAllUsersInChannel(server_response, server->getChannel(channel_name));

			if (!channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "332 " + user->getNickname() + " " + channels[i] + " " + channel->getTopic() + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			if (channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "331 " + user->getNickname() + " " + channels[i] + " :No topic is set" + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			// We send a RPL_NAMREPLY so the first user of the channel can see he is in the channel
			server->sendUserList(channel, user);
		}
	}
	return true;
}
