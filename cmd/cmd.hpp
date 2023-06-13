#ifndef CMD_HPP
#define CMD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "../server/server.hpp"

class Server;

class cmd
{
private:

    std::string _cmd[14];

public:

	cmd();
	cmd(const cmd &rhs);
	cmd &operator=(const cmd &rhs);
	~cmd();

	void	whichCmd(std::string str, Server *server, User *user);
	bool	parsePass(User *user, std::string str, Server *server);
	bool	parseNick(std::string str, Server *server, User *user);
	bool	parseUser(std::string str, Server *server);
	bool	parsePart(std::string str, Server *server, User *user);
	bool	parseTopic(std::string str, Server *server, User *user);
	bool	parseQuit(std::string str, User *user);
	bool	parseMode(std::string str, Server *server, User *user);
	bool	parseNames(std::string str, Server *server);
	bool	parseList(std::string str, Server *server, User *user);
	bool	parseJoin(std::string str, Server *server, User *user);
	bool	parseKick(std::string str, Server *server, User *user);
	bool	parseInvite(std::string str, Server *server, User *user);
	bool	parsePrivmsg(std::string str, Server *server, User *user);
	bool	privMsgInChannel(std::vector<std::string> &arg, Server *server, User *user, int messagePosInt) ;
	bool	privMsgToDirectUser(std::vector<std::string> &arg, Server *server, User *user, int messagePosInt);
	std::string		createServerMessage(User *user, std::string numReply, std::vector<std::string> splitArg);
	std::string		generateErrorMessage(std::string numreply, std::string command);
	void			sendMessageToAllUsersInChannel(std::string message, Channel *channel);
	void			sendMessageToOtherUsersInChannel(std::string message, Channel *channel, User *user);
	void			sendChannelTopicToUser(Channel *channel, User *user);
	void			rebuildMessage(std::vector<std::string> &arg, int index);
	void			rebuildMessageWithIterator(std::vector<std::string> &arg, std::vector<std::string>::iterator messagePos); 
	std::vector<std::string>	splitString(std::string str, const char *delim);
};

#endif