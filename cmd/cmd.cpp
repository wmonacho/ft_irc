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
    _cmd[8] = "NAMES";
    _cmd[9] = "LIST";
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
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2 || server->passwordAlreadyRegistred())
        return (false);
    server->setPassword(splitArg[1]);
    std::string serverResponse = createServerMessage(user, "", splitArg);
    send(user->getSocket(), serverResponse.c_str(), serverResponse.size(), 0);
    return (true);
}

bool    cmd::parseNick(std::string str, Server *server, User *user) //recup le User originaire de la commande
{

    // Parsing de la string (input du user)
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2)
        return (false);
    int i = 0;
    while (splitArg[1][i])
    {
        if (splitArg[1][i] == '/' || splitArg[1][i] == '_' || splitArg[1][i] == '|'|| \
            splitArg[1][i] == '\\' || splitArg[1][i] == '@')
        {
            //numeric_replies
            std::cerr << "Erroneus_nickname" << std::endl;
            return (false);
        }
        i++;
    }

    // Verifier si le NICK n'est pas deja attribue
    if (server->nickAlreadyExist(splitArg[1])) {
        return (false);
	}

    // On envoie la numeric_reply pour confirmer le changement et on fait le changement sur le serveur  
	std::string nick_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + splitArg[0] + " " + splitArg[1] + "\r\n";
	send(user->getSocket(), nick_message.c_str(), nick_message.size(), 0);
	if (splitArg[1].find("\n") != std::string::npos) {
		size_t pos = splitArg[1].find("\n");
		splitArg[1].erase(pos, std::string::npos);
	}
    user->setNickname(splitArg[1]);
    return (true);
}

bool    cmd::parseUser(std::string str, Server *server)
{
    //verifier si le user existe
    std::vector<std::string> splitArg = splitString(str, " ");
    std::cout << "splitArg.size() == " << splitArg.size() << std::endl;
    if (splitArg.size() < 5)
    {
        //envoyer numeric replies
        std::cerr << "USER: not enough parameters" << std::endl;
        return (false);
    }
    if (splitArg.size() > 4 && (splitArg[2] == "0" && splitArg[3] == "*"))
    {
        //setuser
        User    new_user;

        server->createRandomUsername(new_user);
        std::string real_name;
        for (unsigned int i = 4; i < splitArg.size(); i++)
        {
            real_name += splitArg[i];
        }
        new_user.setRealname(real_name);
        server->setUserList(new_user);
        std::cout << "hello from parseUser, it's working bitch" << std::endl;
    }
    return (true);
}

void cmd::whichCmd(std::string str, Server *server, User *user)
{
    std::vector<std::string> arg = splitString(str, " ");
	size_t pos = str.find("\n");
	if (pos != std::string::npos)
		str[pos - 1] = '\0';
    int j = -1;
    for (int i = 0; i < 14; i++)
    {
        if (arg[0] == _cmd[i])
        {
            j = i;
            break;
        }
    }
    switch (j)
    {
        case -1:
            std::cerr << "str: \"" << str << "\" cmd not found" << std::endl;
            return ;

        case 0:
            if (parsePass(user, str, server) == false) // ** USED ON CONNECTION **
            {
                std::cerr << "Usage: PASS [password]" << std::endl;
                return ;
            }
            break;

        case 1:
            if (parseNick(str, server, user) == false)
            {
                std::cerr << "Usage: NICK [nickname]" << std::endl;
                return ;
            }
            break;

        case 2:
            if (parseUser(str, server) == false) // ** USED ON CONNECTION **
            {
                std::cerr << "Usage: USER <user> <mode> <unused> <realname>";
                return ;
            }
            break;

        case 3:
            parseMode(str, server, user);
            break;

        case 4:

            if (parseQuit(str, user) == false)
            {
                std::cerr << "Usage: QUIT [ <Quit Message> ]" << std::endl;
                return ;
            }
            break;

        case 5:
            if (parseJoin(str, server, user) == false)
			{
				// std::cerr << "Usage: JOIN <channel>" << std::endl;
                return ;
			}
            break;

        case 6:

            if (parsePart(str, server, user) == false)
            {
                std::cerr << "Usage: PART <channel> *( \",\" <channel> ) [ <Part Message> ] " << std::endl;
                return ;
            }
            break;

        case 7:

            if (parseTopic(str, server, user) == false)
            {
                std::cerr << "Usage: <channel> [ <topic> ]" << std::endl;
                return ;
            }
            break;

        case 8:
            parseNames(str, server);
            break;

        case 9:
            parseList(str, server, user);
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
	//std::cout << "user_addr :" << &(*server->getChannelUser("channel", "will")) << std::endl;
}

std::string    cmd::createServerMessage(User *user, std::string numReply, std::vector<std::string> splitArg)
{
    std::string tmp;

    if (numReply.empty())
        tmp = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + splitArg[0] + " " + splitArg[1] + "\r\n";
    else
        tmp = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + numReply + " " + splitArg[0] + " " + splitArg[1] + "\r\n";
    std::cout << "SERVER RESPONSE  " << tmp;
    return (tmp);
}

void    cmd::sendMessageToAllUsersInChannel(std::string message, Channel *channel)
{
    if (channel->getUserList().empty()) {
        std::cerr << "MAP IS EMPTY" << std::endl;
        return ;
    }

    std::map<const User*, UserAspects> maptmp = channel->getUserList();
    std::map<const User*, UserAspects>::iterator user = maptmp.begin();
    std::map<const User*, UserAspects>::iterator userEnd = maptmp.end();

    while (user != userEnd) {
        std::string tmp = message;
        send(user->first->getSocket(), tmp.c_str(), tmp.size(), 0);
        user++;
    }
    return ;
}

void    cmd::sendMessageToOtherUsersInChannel(std::string message, Channel *channel, User *user) {

    std::map<const User*, UserAspects> userMap = channel->getUserList();
    std::map<const User*, UserAspects>::iterator userNode = userMap.begin();
    std::map<const User*, UserAspects>::iterator lastUserNode = userMap.end();

    while (userNode != lastUserNode) {
        std::string tmp = message;
        if (userNode->first != user)
            send(userNode->first->getSocket(), tmp.c_str(), tmp.size(), 0);
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