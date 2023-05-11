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
    _cmd[13] = "NOTICE";
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
    _cmd[13] = rhs._cmd[13];
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

bool    cmd::parsePass(std::string str, Server server)
{
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2 || server.alreadyRegistred())
        return (false);
    server.setPassword(splitArg[1]);
    return (true);
}

bool    cmd::parseNick(std::string str, Server server, User user) //recup le User originaire de la commande
{
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
    //verifier que le nick est valide
    if (server.nickAlreadyExist(splitArg[1]))
        return (false);
    user.setNickname(splitArg[1]);
    return (true);
}

bool    cmd::parseUser(std::string str, Server server)
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

        server.createRandomUsername(new_user);
        std::string real_name;
        for (unsigned int i = 4; i < splitArg.size(); i++)
        {
            real_name += splitArg[i];
        }
        new_user.setRealname(real_name);
        server.setUserList(new_user);
        std::cout << "hello from parseUser, it's working bitch" << std::endl;
    }
    return (true);
}

bool    cmd::parseMode(std::string str, Server server, User user)
{
	(void) user;
    std::cout << "Mode cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
    std::vector<std::string> splitArg = splitString(str, " ");
	//MODE <cible> <mode> <argument(s)>
    if (splitArg.size() < 3)
    {
        //envoyer numeric replies
        std::cerr << "MODE: need more params " << std::endl;
        return (false);
    }
    //check si # devant la cible et si le channel existe
	if (splitArg[1][0] != '#' || server.getMap().find(&splitArg[1][1]) == server.getMap().end())
	{
		std::cerr << "MODE: can't find this channel " << std::endl;
        return (false);	
	}
	//check si le User est bien dans la userlist du channel
	if (!server.userIsInChannel(&splitArg[1][1], user))
			return false;
    //check si + ou - devant le mode
	if (splitArg[2][0] != '-' && splitArg[2][0] != '+')
	{
		std::cerr << "MODE: no chan modes " << std::endl;
        return (false);	
	}
    //check si le mode existe (tout depend de ceux que l'on prend)
	std::string modes = "iklmnv";
	if (modes.find(&splitArg[2][1]) == std::string::npos)
	{
		std::cerr << "MODE: unknow mode " << std::endl;
        return (false);	
	}
	//execute les modes +
	for (unsigned int i = 1; splitArg[2][0] == '+' && i < splitArg[2].size(); i++)
	{
		switch(splitArg[2][i] + 48)
		{
			case 105:
				//execute mode i
				break;
			case 107:
				//execute mode k
				break;
			case 108:
				//execute mode l
				break;
			case 109:
				//execute mode m
				break;
			case 110:
				//execute mode n
				break;
			case 118:
				//execute mode v
				break;
		}
	}
	//execute les modes -
	for (unsigned int i = 1; splitArg[2][0] == '-' && i < splitArg[2].size(); i++)
	{
		switch(splitArg[2][i] + 48)
		{
			case 105:
				//execute mode i
				break;
			case 107:
				//execute mode k
				break;
			case 108:
				//execute mode l
				break;
			case 109:
				//execute mode m
				break;
			case 110:
				//execute mode n
				break;
			case 118:
				//execute mode v
				break;
		}
	}
    /*      ERR_NEEDMOREPARAMS              ERR_KEYSET
            ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
            ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE */
	// executer les differents modes
    /*  mode a faire :      +i invite only
                            +m seulent les users admins peuvent parler dans le channel
                            +v donne le droit a un user de parler meme si +m
                            +n empeche les users externe de rejoinde le channel
						   	+l definit une limite de user
                            +k definit un mdp pour le channel
							+ bien d'autres... a voir

    */
   return (true);
}

bool    cmd::parseQuit(std::string str)
{
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() != 2)
        return false;
    //remplacer ça par un throw d'exception ça serait sympa nan ?
    std::cerr << "QUIT: user has quit IRC: " << arg[1] << std::endl;
    return true;
}

bool    cmd::parseJoin(std::string str, Server server, User user)
{
	std::vector<std::string> splitArg = splitString(str, " ");
	if (splitArg.size() != 2)
		return false;
	//check si pas de # devant le channel (jwe crois que l'on peut mettre & aussi a verifier)
	if (splitArg[1][0] != '#')
	{
		std::cerr << " " << std::endl;
		return false;
	}
	std::string channel_name = &splitArg[1][1];
	for (std::map<std::string, Channel>::iterator it = server.getMap().begin(); it != server.getMap().end(); it++)
        if (channel_name == it->second.getName()) {
    /*rejoindre le User dans le Channel deja existant*/
            it->second.setUserList(user);
            return true;
        }
     /*sinon creer un nouveau Channel y ajouter le User avec les droits admin et utiliser setNewChannelInMap ensuite*/
    Channel new_channel(channel_name);
    user.setAdmin(1); //donne les droits admin
    new_channel.setUserList(user); // entre le user dans la list du channel
    server.setNewChannelInMap(new_channel); // entre le channel dans la list des channels du serveur
    std::cout << "Join cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
	return true;
}

bool    cmd::parsePart(std::string str, Server server, User user)
{
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() < 2)
    {
        //numeric reply
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return (false);
    }
    std::vector<std::string> chan = splitString(str, ",");
    std::vector<std::string>::iterator it = chan.begin();
    while (it != chan.end())
    {

        //verifier si le channel existe
		if (!server.channelAlreadyExist(*it))
			return false;
        //verifier si l'user est bien dans le channel
		if (!server.userIsInChannel(*it, user))
			return false;
		//si l'user est bien dans le channel:
        //- ecrire un message annoncant le depart de l'user
		//si le message est dans la commande :
		//sinon afficher
		std::cout << "User lost baby" << std::endl;
        //- delete l'user de la liste du chan
        it++;
    }
    return true;
}

bool    cmd::parseTopic(std::string str, Server server, User user)
{
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() < 2)
    {
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return false;
    }
    //verifier si le channel existe
	if (arg[1][0] == '#' || !server.channelAlreadyExist(&arg[1][1]))
		return false;
    //verifier si le client est dans le channel
	if (!server.userIsInChannel(&arg[1][1], user))
		return false;
    if (arg.size() == 2)
    {
        //verifier que le topic existe
		if (!server.topicAlreadyExist(&arg[1][1]))
			return false;
        //ensuite verifier si l'user est operateur :
		// if (server.getUserAdmin(&arg[1][1], user))
        	//- si oui : set le topic
        //- si non : numeric replies + erreur
        std::cout << "a completer willy" << std::endl;
    }
    return true;
}

/*bool    cmd::parseNames(std::string str)
{
    std::cout << "Names cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;

}

bool    cmd::parseList(std::string str)
{
    std::cout << "List cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseInvite(std::string str)
{
    std::cout << "Invite cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseKick(std::string str)
{
    std::cout << "Kick cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parsePrivmsg(std::string str)
{
    std::cout << "Privmsg cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseNotice(std::string str)
{
    std::cout << "Notice cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}*/

void cmd::whichCmd(std::string cmd, std::string str, Server server, User user)
{
    int j = -1;
    for (int i = 0; i < 14; i++)
    {
        if (cmd == _cmd[i])
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
            if (parsePass(str, server) == false)
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
            if (parseUser(str, server) == false)
            {
                std::cerr << "Usage: USER <user> <mode> <unused> <realname>";
                return ;
            }
            break;

        case 3:
            parseMode(str, server, user);
            break;

        case 4:

            if (parseQuit(str) == false)
            {
                std::cerr << "Usage: QUIT [ <Quit Message> ]" << std::endl;
                return ;
            }
            break;

        /*case 5:
            parseJoin(str, server, user);
            break;
        */
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

        /*case 8:
            parseNames(str);
            break;

        case 9:
            parseList(str);
            break;

        case 10:
             parseInvite(str);
             break;

        case 11:
            parseKick(str);
            break;

        case 12:
            parsePrivmsg(str);
            break;

        case 13:
            parseNotice(str);
            break;*/
    }
}