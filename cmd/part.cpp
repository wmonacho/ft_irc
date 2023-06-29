#include "cmd.hpp"

bool	cmd::parsePart(std::string str, Server *server, User *user)
{
	// Parsing de l'input du user
	std::vector<std::string> splitArg = splitString(str, " ");
	if (splitArg.size() < 2)
	{
		// 461 ERR_NEEDMOREPARAMS
		std::string error = generateErrorMessage("461", splitArg[0]);
		server->getClientData(user->getClientID()).replies.append(error);
		return (false);
	}

	// On verifie que les channels renseignes dans la commande existent bien et que l'utilisateur est dedans
	// et on envoie un PART_reply pour chaque channel a quitter
	std::vector<std::string> channels = splitString(splitArg[1], ",");
	std::vector<std::string> channels_copy = channels;
	std::vector<std::string>::iterator it = channels.begin();
	std::vector<std::string>::iterator it_copy = channels_copy.begin();
	if (splitArg.size() >= 3 && !splitArg[2].empty())
		rebuildMessage(splitArg, 2);
	while (it != channels.end())
	{
		//verifier si le channel existe
		if (server->channelAlreadyExist(*it) == false) {
			std::string error = std::string(":localhost ") + "403 " + user->getNickname() + " " + splitArg[0] + " " + *it_copy + " " + " :No such channel" + "\r\n";
			server->getClientData(user->getClientID()).replies.append(error);
			return false;
		}
		
		//verifier si l'user est bien dans le channel
		if (server->userInChannel(*it, user) == false) {
			std::string error = std::string(":localhost ") + "442" + " " + splitArg[0] + " " + *it_copy + " " + " :You're not on that channel" + "\r\n";
			server->getClientData(user->getClientID()).replies.append(error);
			return false;
		}
		std::string part_message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + splitArg[0] + " ";
		part_message.append(*it_copy);
		if (splitArg.size() >= 3 && !splitArg[2].empty())
			part_message.append(" :" + splitArg[2]);
		part_message.append("\r\n");
		sendMessageToAllUsersInChannel(part_message, server->getChannel(*it), server);
		server->kickUserFromChannel(*it, user);
		if (server->getChannelUserList(*it).size() != 0 && !server->channelHasOperator(*it)) {
			server->setChannelRemplacementOpe((*it));
			std::string rpl_channel_mode_is = ":localhost MODE " + *it + " +o " + server->getChannelUserList(*it).begin()->first->getNickname() + "\r\n";
			this->sendMessageToAllUsersInChannel(rpl_channel_mode_is, server->getChannel(*it), server);
		}
		if (server->getChannelUserList(*it).size() == 0)
			server->deleteChannel(*it);
		it++;
		it_copy++;
	}
	return true;
}