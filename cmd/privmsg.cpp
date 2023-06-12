#include "cmd.hpp"

bool    cmd::parsePrivmsg(std::string str, Server *server, User *user)
{
	// parsing de l'input user
	int	message_check = 0;
	int	number_of_words = 0;
	int	number_of_recipients = 0;
	std::vector<std::string> arg = splitString(str, " ");
	std::string target = arg[1];
	int	sizeWithoutMessage;
	std::vector<std::string>::iterator messagePos;
	std::cout << "ARG SIZE ==> " << arg.size() << std::endl;

	if (arg.size() < 3) {
		std::cerr << "Error: missing paramaters for PRIVMSG command" << std::endl;
		return false;
	}

	int messagePosInt = 0;
	for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++) {
		// On trouve ":" qui signale le debut du message
		if (it->find(":") != std::string::npos) {
			message_check++;
			messagePos = it;
		}
		// On compte le nombre de mots dans le message
		if (message_check > 0) {
			number_of_words++;
		}
		if (message_check == 0)
			messagePosInt++;
	}
	if (message_check != 1) {
		// 412	ERR_NOTEXTTOSEND
		//std::string error = generateErrorMessage("412", arg[0]);
		std::string error = std::string(":localhost ") + "412" + " " + arg[0] + " :No text to send" + "\r\n";
		std::cerr << "412	ERR_NOTEXTTOSEND --> " << message_check << " // " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	std::cout << "Number of words --> " << number_of_words << std::endl;
	sizeWithoutMessage = arg.size() - number_of_words;
	std::cout << "sizeWithoutMessage --> " << sizeWithoutMessage << std::endl;
	if (message_check == 1 && sizeWithoutMessage == 1) {
	    // 411	ERR_NORECIPIENT ":No recipient given (<command>)"
		std::string error = std::string(":localhost ") + "411" + " " + arg[0] + " :No recipient given " + "\r\n";
		std::cerr << "411	ERR_NORECIPIENT --> " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}

	// 407	ERR_TOOMANYTARGETS "<target> :Duplicate recipients. No message \/ delivered"
	for (std::vector<std::string>::iterator it = arg.begin(); it != messagePos; it++) {
		number_of_recipients++;
	}
	int i = 1;
	int j;
	while (i < number_of_recipients) {
		j = 1;
		while (j < number_of_recipients) {
			if (arg[i] == arg[j] && i != j) {
				// 407	ERR_TOOMANYTARGETS "<target> :Duplicate recipients. No message \/ delivered"
				std::string error = std::string(":localhost ") + "407" + " " + arg[0] + " " + arg[i] + " :Duplicate recipients. No message delivered" + "\r\n";
				std::cerr << "407	ERR_NORECIPIENT --> " << error << std::endl;	// DEBUG
				send(user->getSocket(), error.c_str(), error.size(), 0);
				return false;
			}
			j++;
		}
		i++;
	}

    // Cas 1 : le user parle dans un channel
    if (arg[1].find("#") != std::string::npos) {
        arg[1].erase(0, 1);
        if (server->channelAlreadyExist(arg[1])) {
            Channel *channel = server->getChannel(arg[1]);
            arg[1].insert(0, "#");
			rebuildMessageWithIterator(arg, messagePos);
            std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
			sendMessageToOtherUsersInChannel(message, channel, user);
            return true ;
        }
		// 404	ERR_CANNOTSENDTOCHAN
		//std::string error = generateErrorMessage("404", arg[0]);
		std::string error = std::string(":localhost ") + "404" + " " + arg[0] + " " + arg[1] + " :Cannot send to channel" + "\r\n";
		std::cerr << "404	ERR_CANNOTSENDTOCHAN --> " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
    }

	// Cas 2 : le user parle directement a un autre user
    User *dest = server->getUser(target);
	if (!dest) {
		// 401	ERR_NOSUCHNICK "<nickname> :No such nick/channel"
		//std::string error = generateErrorMessage("401", arg[0]);
		std::string error = std::string(":localhost ") + "401" + " " + arg[0] + " " + arg[1] + " :No such nick" + "\r\n";
		std::cerr << "401	ERR_NOSUCHNICK --> " << error << std::endl;	// DEBUG
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	std::cout << "MessageINTPOS ==> " << messagePosInt << std::endl;
	// rebuildMessageWithIterator(arg, messagePos);
	rebuildMessage(arg, messagePosInt);
    std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
    std::string rpl_away = std::string(":localhost ") + "401" + " " + arg[0] + " " + user->getNickname() + " :Message sent" + "\r\n";
	send(dest->getSocket(), message.c_str(), message.size(), 0);
	send(user->getSocket(), rpl_away.c_str(), rpl_away.size(), 0);
	return true;
}

void cmd::rebuildMessageWithIterator(std::vector<std::string> &arg, std::vector<std::string>::iterator messagePos) {

	std::vector<std::string>::iterator it = messagePos + 1;
	while (it != arg.end()) {
		messagePos->append(" ");
		messagePos->append(*it);
		it++;
	}
	messagePos->append("\0");
    return ;
}