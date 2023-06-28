#include "cmd.hpp"

std::string	err_tooManyTargets(std::vector<std::string> &arg, std::vector<std::string>::iterator messagePos);

bool	cmd::parsePrivmsg(std::string str, Server *server, User *user)
{
	/**********************************/
	/*** Parsing de l'input du User ***/
	/**********************************/

	int	sizeWithoutMessage;
	int	message_check = 0;
	int messagePosInt = 0;
	int	number_of_words = 0;
	std::vector<std::string> arg = splitString(str, " ");
	std::string target = arg[1];
	std::vector<std::string>::iterator messagePos;

	if (arg.size() < 3) {
		std::cerr << "Error: missing paramaters for PRIVMSG command" << std::endl;
		return false;
	}

	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++) {
		// On cherche ":" qui signale le debut du message
		if (it->find(":") != std::string::npos) {
			message_check++;
			messagePos = it;
		}
		// On compte le nombre de mots dans le message
		if (message_check > 0) {
			number_of_words++;
		}
		// On trouve la position de la string qui correspond au premier mot du message
		if (message_check == 0)
			messagePosInt++;
	}

	// 412	ERR_NOTEXTTOSEND
	if (message_check != 1) {
		std::string error = std::string(":localhost ") + "412" + " " + arg[0] + " :No text to send" + "\r\n";
		// std::cerr << "412	ERR_NOTEXTTOSEND --> " << message_check << " // " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	// 411	ERR_NORECIPIENT ":No recipient given (<command>)"
	sizeWithoutMessage = arg.size() - number_of_words;
	if (message_check == 1 && sizeWithoutMessage == 1) {
		std::string error = std::string(":localhost ") + "411" + " " + arg[0] + " :No recipient given " + "\r\n";
		// std::cerr << "411	ERR_NORECIPIENT --> " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	if (!server->nickAlreadyExist(arg[1])) {
		// 401 ERR_NOSUCHNICK
		std::string error = std::string(":localhost ") + "401" + " " + arg[0] + " " + arg[1] + " :No such nick" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	
	// 407	ERR_TOOMANYTARGETS "<target> :Duplicate recipients. No message \/ delivered"
	std::string err_tooManyTargets_return = err_tooManyTargets(arg, messagePos);
	if (!err_tooManyTargets_return.empty()) {
		std::string error = std::string(":localhost ") + "407" + " " + arg[0] + " " + err_tooManyTargets_return + " :Duplicate recipients. No message delivered" + "\r\n";
		// std::cerr << "407	ERR_NORECIPIENT --> " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	/**********************************/
	/***  Execution de la commande  ***/
	/**********************************/

    // Cas 1 : le user parle dans un channel
	if (server->channelAlreadyExist(arg[1])) {
		if (privMsgInChannel(arg, server, user, messagePosInt) == false)
			return false;
		return true;
	}

	// Cas 2 : le user parle directement a un autre user
	if (privMsgToDirectUser(arg, server, user, messagePosInt) == false)
		return false;
	return true;
}

std::string	err_tooManyTargets(std::vector<std::string> &arg, std::vector<std::string>::iterator messagePos) {

	int	number_of_recipients = 0;

	for (std::vector<std::string>::iterator it = arg.begin(); it != messagePos; it++) {
		number_of_recipients++;
	}
	int i = 1;
	int j;
	while (i < number_of_recipients) {
		j = 1;
		while (j < number_of_recipients) {
			if (arg[i] == arg[j] && i != j) {
				return arg[i];
			}
			j++;
		}
		i++;
	}
	return "\0";
}

bool	cmd::privMsgInChannel(std::vector<std::string> &arg, Server *server, User *user, int messagePosInt) {

	if (server->channelAlreadyExist(arg[1])) {
		Channel *channel = server->getChannel(arg[1]);
		rebuildMessage(arg, messagePosInt);
		std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
		sendMessageToOtherUsersInChannel(message, channel, user);
		return true ;
	}
	// 404	ERR_CANNOTSENDTOCHAN
	std::string error = std::string(":localhost ") + "404" + " " + arg[0] + " " + arg[1] + " :Cannot send to channel" + "\r\n";
	// std::cerr << "404	ERR_CANNOTSENDTOCHAN --> " << error << std::endl;	// DEBUG
	send(user->getSocket(), error.c_str(), error.size(), 0);
	return false;
}

bool	cmd::privMsgToDirectUser(std::vector<std::string> &arg, Server *server, User *user, int messagePosInt) {
	
	User *dest = server->getUser(arg[1]);
	if (!dest) {
		// 401	ERR_NOSUCHNICK "<nickname> :No such nick/channel"
		std::string error = std::string(":localhost ") + "401" + " " + arg[0] + " " + arg[1] + " :No such nick" + "\r\n";
		// std::cerr << "401	ERR_NOSUCHNICK --> " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	rebuildMessage(arg, messagePosInt);
	std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[1] + " " + arg[messagePosInt] + "\r\n";
	std::string rpl_away = std::string(":localhost ") + "401" + " " + arg[0] + " " + user->getNickname() + " :Message sent" + "\r\n";
	send(dest->getSocket(), message.c_str(), message.size(), 0);
	send(user->getSocket(), rpl_away.c_str(), rpl_away.size(), 0);
	return true;
}