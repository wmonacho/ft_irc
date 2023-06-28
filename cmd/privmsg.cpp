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
	int	findMessage;
	std::vector<std::string> arg = splitString(str, " ");
	std::vector<std::string>::iterator messagePos;

	if (arg.size() < 3) {
		std::string error = std::string(":localhost ") + " " + arg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	// 412	ERR_NOTEXTTOSEND
	if (str.find(":") == std::string::npos) {
		std::string error = std::string(":localhost ") + "412"  + " :No text to send" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	findMessage = str.find(":");
	std::string	msg = str.substr(findMessage);
	std::string target = arg[1];

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

	// 411	ERR_NORECIPIENT ":No recipient given (<command>)"
	sizeWithoutMessage = arg.size() - number_of_words;
	if (message_check == 1 && sizeWithoutMessage == 1) {
		std::string error = std::string(":localhost ") + "411" + " :No recipient given " + arg[0] + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	
	// 407	ERR_TOOMANYTARGETS "<target> :Duplicate recipients. No message \/ delivered"
	std::string err_tooManyTargets_return = err_tooManyTargets(arg, messagePos);
	if (!err_tooManyTargets_return.empty()) {
		std::string error = std::string(":localhost ") + "407" + " " + arg[0] + " " + err_tooManyTargets_return + " :Duplicate recipients. No message delivered" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	/**********************************/
	/***  Execution de la commande  ***/
	/**********************************/

    // Cas 1 : le user parle dans un channel
	if (server->channelAlreadyExist(arg[1])) {
		if (privMsgInChannel(arg, server, user, msg) == false)
			return false;
		return true;
	}

	// Cas 2 : le user parle directement a un autre user
	if (privMsgToDirectUser(arg, server, user, msg) == false)
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

bool	cmd::privMsgInChannel(std::vector<std::string> &arg, Server *server, User *user, std::string msg) {

	if (server->channelAlreadyExist(arg[1])) {
		Channel *channel = server->getChannel(arg[1]);
		std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[1] + " " + msg + "\r\n";
		sendMessageToOtherUsersInChannel(message, channel, user);
		return true ;
	}
	// 404	ERR_CANNOTSENDTOCHAN
	std::string error = std::string(":localhost ") + "404" + " " + arg[0] + " " + arg[1] + " :Cannot send to channel" + "\r\n";
	send(user->getSocket(), error.c_str(), error.size(), 0);
	return false;
}

bool	cmd::privMsgToDirectUser(std::vector<std::string> &arg, Server *server, User *user, std::string msg) {
	
	User *dest = server->getUser(arg[1]);
	if (!dest) {
		// 401	ERR_NOSUCHNICK "<nickname> :No such nick/channel"
		std::string error = std::string(":localhost ") + "401" + " " + arg[0] + " " + arg[1] + " :No such nick" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + arg[0] + " " + arg[1] + " " + msg + "\r\n";
	std::string rpl_away = std::string(":localhost ") + "401" + " " + arg[0] + " " + user->getNickname() + " :Message sent" + "\r\n";
	send(dest->getSocket(), message.c_str(), message.size(), 0);
	send(user->getSocket(), rpl_away.c_str(), rpl_away.size(), 0);
	return true;
}