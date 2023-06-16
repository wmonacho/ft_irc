#include "cmd.hpp"

bool	cmd::parseJoin(std::string str, Server *server, User *user)
{
	// Parsing de la string (commande + argument )
	std::vector<std::string> splitArg = splitString(str, " ");
	if (splitArg.size() < 2) {
		// 461 ERR_NEEDMOREPARAMS
		std::string error = std::string("localhost :") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	std::vector<std::string> channels = splitString(splitArg[1], ",");
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i][0] != '#' && channels[i][0] != '&') {
			// 476	ERR_BADCHANNELMASK
			std::string error = std::string("localhost :") + "476 " + user->getNickname() + " " + &channels[i][1] + " :Bad Channel Mask" + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
			return false;
		}
	}

	std::string server_response = createServerMessage(user, "", splitArg);

	for (size_t i = 0; i < channels.size(); i++) {
		std::string channel_name = &channels[i][1];
		std::string server_response = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + splitArg[0] + " " + channels[i] + "\r\n";
		
		// Cas 1 : le channel existe, donc y ajoute le user et on envoie le message de "bienvenue" a tout le monde
		// + on envoie le topic (s'il existe) et la liste des USER 
		if (server->channelAlreadyExist(channel_name)) {

			Channel* channel = server->getChannel(channel_name);

			if (channel->getInviteOnly()) {
				// 473    ERR_INVITEONLYCHAN
			  std::string error = std::string("localhost :") + "473 " + user->getNickname() + " " + channels[i] + " :Cannot join channel (+i)" + "\r\n";
			  send(user->getSocket(), error.c_str(), error.size(), 0);
			  continue;
			}

			if (splitArg.size() < 3 && channel->getPassword() != "") {
				// 475    ERR_BADCHANNELKEY
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
			sendMessageToAllUsersInChannel(server_response, channel);

			// On envoie le topic s'il existe
			if (!channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "332 " + user->getNickname() + " " + channels[i] + " :" + channel->getTopic() + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			if (channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "331 " + user->getNickname() + " " + channels[i] + " :Not topic is set" + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
			}

			// liste des users
			if (channel->getUserList().size() > 0) {

				// On envoie une RPL_NAMEPLY
				std::map<const User *, UserAspects> map = channel->getUserList();
				std::map<const User *, UserAspects>::iterator userInChannel = map.begin();

				for (;userInChannel != map.end(); userInChannel++) {
					std::cout << "Sending user list : " << userInChannel->first->getUsername() << std::endl;
					std::string user_list = std::string(":localhost ") + "353" + " " + "NAMES" + " " + userInChannel->first->getUsername() + "\r\n";
					send(user->getSocket(), user_list.c_str(), user_list.size(), 0);
				}

				// On envoie une RPL_ENDOFNAMES pour signaler la fin de la liste des users dans le channel
				std::string end_of_names = std::string(":localhost ") + "366" + " " + "NAMES" + "\r\n";
				send(user->getSocket(), end_of_names.c_str(), end_of_names.size(), 0);
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
			//std::string user_list = std::string(":localhost ") + "353 " + user->getNickname() + " == " + channels[i] + " :" + user->getNickname();
			//send(user->getSocket(), user_list.c_str(), user_list.size(), 0);
		}
	}
	return true;
}

//:localhost 353 user == #channel :user1 user2...
//:localhost 366 user #channel :End of NAMES list

// Users on :ebrodeur: issou
// Test cote serveur, on envoie bien ca :
	// :localhost 353 coucou == #lol :ebrodeur issou coucou

	// ERR_BANNEDFROMCHAN
	// ERR_INVITEONLYCHAN
	// ERR_CHANNELISFULL
	// ERR_BADCHANMASK
	// ERR_NOSUCHCHANNEL
	// ERR_TOOMANYCHANNELS
