#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../user/User.hpp"
#include "../server/server.hpp"
#include "../channel/Channel.hpp"

class User;
class Server;
class Channel;

class bot
{
private:

	int _botSocket;

public:

	bot();
	~bot();

	int		startBot(int socketFd);
	void	sendFromBot();
	int		runBot(char *buffer, User *user, Server *server);
	void	sendBotMessageToOtherUsersInChannel(std::string message, Channel *channel, User *user);

	int		getBotSocket();
};

#endif