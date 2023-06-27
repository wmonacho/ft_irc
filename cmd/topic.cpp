#include "cmd.hpp"

bool	cmd::parseTopic(std::string str, Server *server, User *user)
{
	std::vector<std::string> arg = splitString(str, " ");

	// 461  ERR_NEEDMOREPARAMS
	if (arg.size() < 2)
	{
		std::string error = generateErrorMessage("461", arg[0]);
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	Channel *channel = server->getChannel(arg[1]);
	if (!channel) {
		std::cerr << "Error: Channel not found" << std::endl;
		return false;
	}
	// 442 ERR_NOTONCHANNEL "<channel> :You're not on that channel"
	if (channel->getUserList().find(user) == channel->getUserList().end()) {
		std::string error = std::string(":localhost ") + "442" + " " + arg[0] + " " + channel->getName() + " : No topic is set" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false; 
	}
	// 482	ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
   // std::cout << std::endl << std::endl << std::endl << std::endl << "cocu" << std::endl << std::endl << std::endl << std::endl << std::endl;
	if (channel->getTopicAdmin() && !channel->getUserAdmin(user) && arg.size() >= 3) {
		std::string error = std::string(":localhost ") + "482" + " " + arg[0] + " " + channel->getName() + " :You're not channel operator" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
		return false;
	}
	// Cas 1 : on renvoie le topic s'il existe
	if (arg.size() <= 2) {
		std::string topic_message;
		if (channel->getTopic().empty()) {
			topic_message = std::string(":localhost ") + "331" + " " + user->getNickname() + " " + channel->getName() + " : No topic is set" + "\r\n";
		}
		else
			topic_message = std::string(":localhost ") + "332" + " " + user->getNickname() + " " + channel->getName() + " " + channel->getTopic() + "\r\n";
		send(user->getSocket(), topic_message.c_str(), topic_message.size(), 0);
		return true;
	}
	// Cas 2 : on set le nouveau topic
	if (arg.size() >= 3) {
		rebuildMessage(arg, 2);
		// Si on a que ":" alors il faut clear le topic
		if (arg[2].size() == 1 && (arg[2].find(":") != std::string::npos)) {
			channel->setTopic("");
			return true;
		}
		std::string new_topic = arg[2];
		std::cout << "Setting up new topic --> " << new_topic << std::endl;
		channel->setTopic(new_topic);
		if (!channel->getTopic().empty()) {
				std::string topic = std::string(":localhost ") + "332 " + user->getNickname() + " " + channel->getName() + " " + channel->getTopic() + "\r\n";
				send(user->getSocket(), topic.c_str(), topic.size(), 0);
		}
		return true ;
	}
	return true;
}