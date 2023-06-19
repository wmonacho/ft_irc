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

class User;

class bot
{
private:

    int _botSocket;
public:

       bot();
	~bot();

	int	startBot(int socketFd);
	void	sendFromBot();
	int	runBot(char* buffer, User *user);
};

#endif