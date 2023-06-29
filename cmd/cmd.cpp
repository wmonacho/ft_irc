#include "cmd.hpp"
#include "../server/server.hpp"

cmd::cmd()
{
	_cmd[0] = "PASS";
	_cmd[1] = "NICK";
	_cmd[2] = "USER";
	_cmd[3] = "MODE";
	_cmd[4] = "QUIT";
	_cmd[5] = "JOIN";
	_cmd[6] = "PART";
	_cmd[7] = "TOPIC";
	_cmd[8] = "WHO";
	_cmd[9] = "CAP";
	_cmd[10] = "INVITE";
	_cmd[11] = "KICK";
	_cmd[12] = "PRIVMSG";
}

cmd::cmd(const cmd &rhs)
{
	*this = rhs;
}

cmd &cmd::operator=(const cmd &rhs)
{
	_cmd[0] = rhs._cmd[0];
	_cmd[1] = rhs._cmd[1];
	_cmd[2] = rhs._cmd[2];
	_cmd[3] = rhs._cmd[3];
	_cmd[4] = rhs._cmd[4];
	_cmd[5] = rhs._cmd[5];
	_cmd[6] = rhs._cmd[6];
	_cmd[7] = rhs._cmd[7];
	_cmd[8] = rhs._cmd[8];
	_cmd[9] = rhs._cmd[9];
	_cmd[10] = rhs._cmd[10];
	_cmd[11] = rhs._cmd[11];
	_cmd[12] = rhs._cmd[12];
	return (*this);
}

cmd::~cmd()
{

}

void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out)
{
	char *token = strtok(const_cast<char*>(str.c_str()), delim);
	while (token != NULL)
	{
		out.push_back(std::string(token));
		token = strtok(NULL, delim);
	}
}

std::vector<std::string> cmd::splitString(std::string str, const char *delim)
{
	std::vector<std::string> out;
	tokenize(str, delim, out);
	return (out);
}

bool    cmd::parsePass(User *user, std::string str, Server *server)
{
	(void)user;
	(void)str;
	(void)server;
	return (true);
}

bool    cmd::parseNick(std::string str, Server *server, User *user) //recup le User originaire de la commande
{
	(void)str;
	(void)server;
	(void)user;
	return (true);
}

bool    cmd::parseUser(std::string str, Server *server)
{

	(void)str;
	(void)server;
	return (true);
}

int	cmd::whichCmd(int clientID, Server *server, User *user)
{
	std::string str = server->getClientData(clientID).dataString;
	std::vector<std::string> arg = splitString(str, " ");
	size_t pos = arg[0].find("\n");
	if (pos != std::string::npos)
		arg[0].erase(pos - 1, arg[0].size() - 1);
	pos = str.find("\n");
	if (pos != std::string::npos)
		str[pos - 1] = '\0';
	
	//arg[0].erase(arg[0].find("\r"), arg[0].find("\r") + 1);
	int j = -1;
	for (int i = 0; i < 14; i++)
	{
		if (arg[0] == _cmd[i])
		{
			j = i;
			break;
		}
	}
	std::string response;
	switch (j)
	{
		case -1:
		    response = std::string(":localhost ") + "421 " + user->getNickname() + " " + arg[0] + " :Unknown command" + "\r\n";
			server->getClientData(clientID).replies.append(response);
		    return 1;

		 case 0:
		 	if (parsePass(user, str, server) == false) // ** USED ON CONNECTION **
		 	{
		 		std::cerr << "Usage: PASS [password]" << std::endl;
		 		return 1;
		 	}
		 	break;

		 case 1:
		 	if (parseNick(str, server, user) == false)
		 	{
		 		std::cerr << "Usage: NICK [nickname]" << std::endl;
		 		return 1;
		 	}
		 	break;

		case 2:
		 	if (parseUser(str, server) == false) // ** USED ON CONNECTION **
			{
			     std::cerr << "Usage: USER <user> <mode> <unused> <realname>";
			     return 1;
		 	}
			break;

		case 3:
			if (parseMode(str, server, user) == false) {
				return 1;
			}
			break;

		case 4:

			if (parseQuit(server, str, user) == false)
			{
				std::cerr << "Usage: QUIT [ <Quit Message> ]" << std::endl;
				return 1;
			}
			return 2;

		case 5:
			if (parseJoin(str, server, user) == false)
			{
				// std::cerr << "Usage: JOIN <channel>" << std::endl;
				return 1;
			}
			break;

		case 6:

			if (parsePart(str, server, user) == false)
			{
				std::cerr << "Usage: PART <channel> *( \",\" <channel> ) [ <Part Message> ] " << std::endl;
				return 1;
			}
			break;

		case 7:

			if (parseTopic(str, server, user) == false)
			{
				std::cerr << "Usage: <channel> [ <topic> ]" << std::endl;
				return 1;
			}
			break;
		case 8:
			break;
		case 9:
			 break;
		case 10:
			 parseInvite(str, server, user);
			 break;

		case 11:
			parseKick(str, server, user);
			break;

		case 12:
			parsePrivmsg(str, server, user);
			break;
	}
	return 0;
}

std::string    cmd::createServerMessage(User *user, std::string numReply, std::vector<std::string> splitArg)
{
	std::string tmp;

	if (numReply.empty())
		tmp = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + splitArg[0] + " " + splitArg[1] + "\r\n";
	else
		tmp = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost " + numReply + " " + splitArg[0] + " " + splitArg[1] + "\r\n";
	return (tmp);
}

void    cmd::sendMessageToAllUsersInChannel(std::string message, Channel *channel, Server *server)
{
	if (!channel) {
		std::cerr << "Error: channel does not exist" << std::endl;
		return ;
	}

	std::map<const User*, UserAspects> maptmp = channel->getUserList();
	std::map<const User*, UserAspects>::iterator user = maptmp.begin();
	std::map<const User*, UserAspects>::iterator userEnd = maptmp.end();

	while (user != userEnd) {
		std::string tmp = message;
		//send(user->first->getSocket(), tmp.c_str(), tmp.size(), 0);
		server->getClientData(user->first->getClientID()).replies.append(tmp);
		user++;
	}
	return ;
}

void    cmd::sendMessageToOtherUsersInChannel(std::string message, Channel *channel, User *user, Server *server) {

	std::map<const User*, UserAspects> userMap = channel->getUserList();
	std::map<const User*, UserAspects>::iterator userNode = userMap.begin();
	std::map<const User*, UserAspects>::iterator lastUserNode = userMap.end();

	while (userNode != lastUserNode) {
		std::string tmp = message;
		if (userNode->first != user)
		{
			server->getClientData(userNode->first->getClientID()).replies.append(tmp);
		}
		userNode++;
	}
	return ;
}

void cmd::rebuildMessage(std::vector<std::string> &arg, int index) {

	size_t i = index + 1;
	while (i < arg.size()) {
		arg[index].append(" ");
		arg[index].append(arg[i]);
		i++;
	}
	arg[index].append("\0");
	return ;
}

std::string cmd::generateErrorMessage(std::string numReply, std::string command) {

	std::string error = std::string(":localhost ") + numReply + " " + command + "\r\n";
	return error;
}