#include "cmd.hpp"

bool	cmd::parseJoin(std::string str, Server *server, User *user)
{
	// Parsing de la string (commande + argument )
	std::vector<std::string> splitArg = splitString(str, " ");

	if (splitArg.size() < 2) {
		// 461	ERR_NEEDMOREPARAMS
		std::string error = std::string("localhost :") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	std::vector<std::string> channels = splitString(splitArg[1], ",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i][0] != '#' && channels[i][0] != '&') {
			// 476	ERR_BADCHANNELMASK
			std::string error = std::string("localhost :") + "476 " + user->getNickname() + " " + channels[i] + " :Bad Channel Mask" + "\r\n";
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
			  	std::string error = std::string("localhost :") + "473 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+i)" + "\r\n";
			  	send(user->getSocket(), error.c_str(), error.size(), 0);
			  	continue;
			}

			if (channel->getUserList().find(user) != channel->getUserList().end())
			{
				std::cout << "USER ALDREADY ON THIS CHANNEL" << std::endl;
			}
			else 
				std::cout << "USER NOT YET ON THIS CHANNEL" << std::endl;

			if (splitArg.size() < 3 && channel->getPassword() != "") {
				// 475	ERR_BADCHANNELKEY
				std::string error = std::string("localhost :") + "475 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+k)" + "\r\n";
				send(user->getSocket(), error.c_str(), error.size(), 0);
				continue;
			}

			if (splitArg.size() > 2 && channel->getPassword() != "")
			{
				std::vector<std::string> passwords = splitString(splitArg[2], ",");
				if (passwords.size() < i) {
					//475    ERR_BADCHANNELKEY
					std::string error = std::string("localhost :") + "475 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+k)" + "\r\n";
					send(user->getSocket(), error.c_str(), error.size(), 0);
					continue;
				}
				if (passwords[i] != channel->getPassword())
				{
					//475    ERR_BADCHANNELKEY
					std::string error = std::string("localhost :") + "475 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+k)" + "\r\n";
					send(user->getSocket(), error.c_str(), error.size(), 0);
					continue;
				}
			}

			if (!server->channelEnoughSpace(channel->getName())) {
				// 471 ERR_CHANNELISFULL
				std::string error = std::string("localhost :") + "471 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+l)" + "\r\n";
				send(user->getSocket(), error.c_str(), error.size(), 0);
				continue;
			}

 			UserAspects	new_aspects(0);

			server->addUserToChannel(channel_name, user, new_aspects);
			this->sendMessageToAllUsersInChannel(server_response, channel);

			// On envoie le topic s'il existe
			if (!channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "332 " + user->getNickname() + " " + channels[i] + " :" + channel->getTopic() + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			if (channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "331 " + user->getNickname() + " " + channels[i] + " :Not topic is set" + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			//if (server->getChannelUserAdmin(channel_name, user))
			//	std::cout << user->getNickname() << " is Admin" << std::endl;
			//else
			//	std::cout << user->getNickname() << " is not Admin" << std::endl;

			// User list : we send a RPL_NAMREPLY
			if (channel->getUserList().size() > 0) {

				std::map<const User *, UserAspects> map = channel->getUserList();

				// Create RPL_NAMREPLY string
				std::string user_list = std::string(":localhost ") + "353 " + user->getNickname() + " = " + channel_name + " :";
				
				// Loop to append all of the nicknames of all the users present in the channel
				for (std::map<const User *, UserAspects>::iterator userInChannel = map.begin(); userInChannel != map.end(); userInChannel++) {

					if (userInChannel->second.getAdmin())
						user_list.append("@");
					user_list.append(userInChannel->first->getNickname());
					if (userInChannel != --map.end())
						user_list.append(" ");
				}

				user_list.append("\r\n");
				std::cout << "USER_LIST = " << user_list << std::endl;
				send(user->getSocket(), user_list.c_str(), user_list.size(), 0);
			}
			continue;
		}

		// Cas 2 : le channel n'existe pas, il faut donc le creer dans notre serveur et y ajouter l'utilisateur
		if (!server->channelAlreadyExist(channel_name)) {

			UserAspects	new_aspects(0);
			Channel *channel = new Channel(channel_name);

			server->createNewChannel(channel_name, channel);
			if (!server->getChannel(channel_name))
				return false;
			server->addUserToChannel(channel_name, user, new_aspects);
			sendMessageToAllUsersInChannel(server_response, server->getChannel(channel_name));

			// We send a RPL_NAMREPLY so the first user of the channel can see he is in the channel
			std::string user_list = std::string(":localhost ") + "353 " + user->getNickname() + " = " + channel_name + " :" + user->getNickname() + "\r\n";
			send(user->getSocket(), user_list.c_str(), user_list.size(), 0);

			//if (server->getChannelUserAdmin(channel_name, user))
			//	std::cout << user->getNickname() << " is Admin" << std::endl;
			//else
			//	std::cout << user->getNickname() << " is not Admin" << std::endl;
		}
	}
	return true;
}
