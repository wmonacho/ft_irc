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
    if (server->nickAlreadyExist(splitArg[1]))
        return (false);
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


bool    cmd::parseMode(std::string str, Server *server, User *user)
{
    //mode i, t, k, o
    std::vector<std::string> splitArg = splitString(str, " ");
	//MODE <cible> <mode> <argument(s)>
    if (splitArg.size() < 3)
    {
        //envoyer numeric replies
        std::cerr << "MODE: need more params " << std::endl;
        return (false);
    }
    //check si # devant la cible et si le channel existe
	if (splitArg[1][0] != '#' || server->getMap().find(&splitArg[1][1]) == server->getMap().end())
	{
           std::cerr << "MODE: can't find this channel " << std::endl;
           return (false);
	}
	//check si le User est bien dans la userlist du channel
	if (!server->userInChannel(&splitArg[1][1], user))
           return false;
    //check si + ou - devant le mode
	if ((splitArg[2][0] != '-' && splitArg[2][0] != '+') || splitArg[2].size() != 2)
	{
           std::cerr << "MODE: no chan modes " << std::endl;
           return (false);
	}
    //check si le mode existe (tout depend de ceux que l'on prend)
	std::string modes = "ikto";
	if (modes.find(&splitArg[2][1]) == std::string::npos)
	{
           std::cerr << "MODE: unknow mode " << std::endl;
           return (false);
	}

       Channel *chan = server->getChannel(splitArg[1]);
       const User* u = server->getChannelUser(splitArg[1], splitArg[3]);
    //execute les modes +
    //il restera a modifier les fonctions affectees par les modes
	for (unsigned int i = 1; splitArg[2][0] == '+' && i < splitArg[2].size(); i++)
	{
		const User* u = server->getChannelUser(splitArg[1], splitArg[3]);
		switch(splitArg[2][i] + 48)
		{
                  case 105:
                    	//execute mode i
                    	chan->setInviteOnly(true);
                    	break;
                  case 107:
                    	//execute mode k
                    	chan->setPassword(splitArg[3]);
                    	break;
                  case 108:
                    	chan->setUserLimit(atoi(splitArg[3].c_str()));
                    	break;
                      	//execute mode L
                  case 111:
                      	//execute mode o
						chan->changeUserAdmin(u, true);
                      	break;
                  case 116:
                      	//execute mode t
                      	chan->setTopicAdmin(true);
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
                      chan->setInviteOnly(false);
                      break;
                  case 107:
                      //execute mode k
                      chan->setPassword("");
                      break;
                  case 108:
                      //execute mode L
                      chan->setUserLimit(-1);
                      break;
                  case 111:
                      //execute mode o
                      chan->changeUserAdmin(u, false);
                      break;
                  case 116:
                      chan->setTopicAdmin(false);
                      break;
		}
	}
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

bool    cmd::parseJoin(std::string str, Server *server, User *user)
{
 	std::vector<std::string> splitArg = splitString(str, " ");
 	if (splitArg.size() != 2)
 		return false;
 	//check si pas de # devant le channel (jwe crois que l'on peut mettre & aussi a verifier)
 	if (splitArg[1][0] != '#')
 	{
 		std::cerr << "ERR_BADCHANNELKEY" << std::endl;
 		return false;
 	}
 	std::string channel_name = &splitArg[1][1];
    std::string server_response = createServerMessage(user, "", splitArg);
    if (server->channelAlreadyExist(channel_name))
	{
	    Channel* channel = server->getChannel(channel_name);
 		UserAspects	new_aspects(0);
		// channel->setUserList(user, new_aspects);
        server->addUserToChannel(channel_name, server->getUser(user->getNickname()), new_aspects);
        // We send a message to all the users connected to the channel
        sendResponseToAllUsersInChannel(server_response, channel);
		return true;
	}
    /*sinon creer un nouveau Channel y ajouter le User avec les droits admin et utiliser setNewChannelInMap ensuite*/
 	UserAspects	new_aspects(1);
    Channel *channel = new Channel(channel_name);
    server->createNewChannel(channel_name, *channel);
	if (!server->getChannel(channel_name))
		return false;
	server->addUserToChannel(channel_name, server->getUser(user->getNickname()), new_aspects);
    // After the channel creation (if it didn't exist)
	//std::cout << "user_addr server :" << &(*server->getChannelUser(channel_name, user)) << std::endl;
	//std::cout << "user_addr channel :" << &(*channel->getUser(user)) << std::endl;
    sendResponseToAllUsersInChannel(server_response, server->getChannel(channel_name));
 	return true;
}

bool    cmd::parsePart(std::string str, Server *server, User *user)
{
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() < 2)
    {
        //numeric reply
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return (false);
    }
    std::vector<std::string> chan = splitString(splitArg[1], ",");
    std::vector<std::string>::iterator it = chan.begin();
    while (it != chan.end())
    {
	 //verifier si le channel existe
	 if (!server->channelAlreadyExist(*it))
	     return false;
        //verifier si l'user est bien dans le channel
	 if (!server->userInChannel(*it, user))
	     return false;
	 else
	 {
	     std::cout << "user -> " << user->getNickname() << "has left the channel ->" << *it;
	     if (splitArg.size() >= 3)
	     {
		  std::cout << ": ";
		  std::vector<std::string>::iterator it = splitArg.begin() + 2;
		  while (it != splitArg.end())
		  {
		      std::cout << *it << " ";
		      it++;
		  }
	     }
	     server->kickUserFromChannel(*it, user);
	 }
	 it++;
    }
    return true;
}

bool    cmd::parseTopic(std::string str, Server *server, User *user)
{
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() < 3)
    {
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return false;
    }
    //verifier si le channel existe
    if (arg[1][0] == '#' || !server->channelAlreadyExist(&arg[1][1]))
        return false;
    //verifier si le client est dans le channel
    if (!server->userInChannel(arg[1], user))
        return false;
    if (arg.size() == 3)
    {
        Channel *chan = server->getChannel(arg[1]);
        if (chan->getTopicAdmin())
        {
            if (!chan->getUserAdmin(user))
            {
                std::cerr << "User has to be admin to edit topic" << std::endl;
                return false;
            }
            else
            {
                chan->setTopic(arg[3]);
                return true;
            }
        }
        if (arg[2] == "")
        {
            chan->setTopic("");
            return true;
        }
        else
        {
            chan->setTopic(arg[2]);
            return true;
        }
    }
    return true;
}

bool    cmd::parseNames(std::string str, Server *server)
{
    std::vector<std::string> arg = splitString(str, " ");

    std::map<std::string, Channel> map = server->getMap();
    std::vector<User> copy_list_user = server->getUserList();

    if (arg.size() == 1)
    {
        for (std::map<std::string, Channel>::iterator it = map.begin(); it != map.end(); it++)
        {
            //checkez si channel secrete ou pas
            std::cout << "Channel: " << it->first << std::endl;
            std::map<const User*, UserAspects> userlist = it->second.getUserList();
            for (std::map<const User*, UserAspects>::iterator itUser = userlist.begin(); itUser != userlist.end(); itUser++)
            {
                std::cout << itUser->first->getUsername() << std::endl;
                std::vector<User>::iterator iter = std::find(copy_list_user.begin(), copy_list_user.end(), itUser->first);
                if (iter != copy_list_user.end())
                    copy_list_user.erase(iter);
            }
        }
        for (std::vector<User>::iterator cpyIt = copy_list_user.begin(); cpyIt != copy_list_user.end(); cpyIt++)
            std::cout << (*cpyIt).getUsername() << std::endl;
    }
    else
    {
        std::vector<std::string> chan = splitString(arg[1], ",");
        //print les channels passes en parametre ainsi que leurs users respectifs tout en faisant attention
        //a ne pas afficher les utilisateurs qui ne sont pas visibles.
    }
    //petite boucle while pour faire les checks
    return true;
}

bool    cmd::parseList(std::string str, Server *server)
{
    std::vector<std::string> arg = splitString(str, " ");
    std::map<std::string, Channel> map = server->getMap();
    std::vector<std::string> chans;

    if (arg.size() == 1)
    {
        for (std::map<std::string, Channel>::iterator it = map.begin(); it != map.end(); it++)
        {
            std::cout << "Channel: " << it->second.getName() << std::endl;
            std::cout << " - topic: " << it->second.getTopic() << std::endl;
            std::cout << std::endl;
        }
        return true;
    }
    arg.erase(arg.begin());
    chans = splitString(arg[1], ",");
    for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
    {
        std::map<std::string, Channel>::iterator itMap = map.begin();
        int isValid = 0;
        while (itMap != map.end())
        {
            if (itMap->second.getName() == *it)
            {
                std::cout << "Channel: " << itMap->second.getName() << std::endl;
                std::cout << " - topic: " << itMap->second.getTopic() << std::endl;
                isValid = 1;
            }
            else
                itMap++;
        }
        if (isValid == 0)
        {
            std::cerr << "Channel: " << itMap->second.getName() << "doesn't exist."  << std::endl;
            return false;
        }
    }
    return true;
}

bool    cmd::parseInvite(std::string str, Server *server, User *user)
{
    std::vector<std::string> arg = splitString(str, " ");

    if (arg.size() != 3)
        return false;
    std::string nick = arg[1];
    std::string chan = &arg[2][1];
    std::map<std::string, Channel> map = server->getMap();
    int chanFound = 0;
    for (std::map<std::string, Channel>::iterator it = map.begin(); it != map.end(); it++)
    {
        if (it->first == chan)
        {
            chanFound = 1;
            break ;
        }
    }
    //channel pas valide = pas d erreur
    if (chanFound == 0)
        return false;
    if (server->nickAlreadyExist(nick) == false)
    {
        std::cerr << "ERR_NOSUCHNICK" << std::endl;
        return false;
    }
    //seulement les users du channel peuvent inviter des gens
    //verifier que l'user soit bien dans le channel
	if (!server->userInChannel(chan, user))
	{
		std::cerr << "ERR_NOTONCHANNEL" << std::endl;
		return false;
	}
    //si le channel a le flag invite only set, seulement les channels operators peuvent inviter
	if (!server->getChannelUserAdmin(chan, user) && server->channelIsInviteOnly(chan))
	{
		std::cerr << "ERR_CHANOPRIVSNEEDED" << std::endl;
		return false;
	}
    //checker si l'user est deja sur le chan
	if (server->userInChannel(chan, server->getChannelUser(chan, nick)))
	{
		std::cerr << "ERR_USERONCHANNEL" << std::endl;
		return false;
	}
	//check si le channel a une limit
	if (server->channelHaveLimit(chan))
	{
		//check si la limit est inferieur ou egal aux users sur le channel
		if (!server->channelEnoughSpace(chan))
		{
			std::cerr << "ERR_BEYONDTHELIMIT" << std::endl;
			return false;
		}
	}
	//execution de la cmd: envoyer le nouveau User dans le channel
	UserAspects channel_aspects(false);
	server->addUserToChannel(chan, server->getConstUser(nick), channel_aspects);
    return true;
}

bool    cmd::parseKick(std::string str, Server *server)
{
    std::cout << "Kick cmd found" << std::endl;
    std::vector<std::string> arg = splitString(str, " ");

    //check si au moins 3 param : need more params
    if (arg.size() < 3)
    {
        std::cerr << "Error: need more params" << std::endl;
        return false;
    }
    //check si # ou & devant le server
    if (arg[1][0] != '#' && arg[1][0] != '&')
    {
        std::cerr << "Error: bad channel mask" << std::endl;
        return false;
    }
	//check si plusieurs channel dans la cmd et si bad mask #
	//for (int i = 1; i < arg.size(); i++)
	//{
	//if (arg[i][0] != '#' && arg[i][0]  != '&' && )
	//}
    //check si le server existe
    if (!server->channelAlreadyExist(&arg[1][1]))
    {
        std::cerr << "Error: no such channel" << std::endl;
        return false;
    }
    //check si le User exist dans le channel
    if (!server->userInChannel(&arg[1][1], server->getChannelUser(&arg[1][1], arg[2])))
	{
		std::cerr << "Error: not on channel" << std::endl;
		return false;
	}
    //check si il y a un commentaire : si oui l'afficher
	if (arg.size() > 3)
	{
		if (arg[3][0] != ':')
		{
			std::cerr << "Error: wrong parameters" << std::endl;
			return false;
		}
		//bannir le User du channel
		server->kickUserFromChannel(&arg[1][1], server->getChannelUser(&arg[1][1], arg[2]));
		//server->kickChannelUser(&arg[2][1], kick_user);
		//ecris le commentaire de kick
		unsigned int i = 3;
		while (i < arg.size() - 1)
		{
			std::cout << arg[i] << " ";
			i++;
		}
		std::cout << arg[i] << std::endl;
		return true;
	}
	//bannir le User du channel
	server->kickUserFromChannel(&arg[1][1], server->getChannelUser(&arg[1][1], arg[2]));
	//ecris la phrase de kick par default
	std::cout << "You're banned, go find somewhere else to be." << std::endl;
    return true;
}


bool    cmd::parsePrivmsg(std::string str, Server *server, User *user)
{
    // Parameters: <msgtarget> <text to be sent>
	// Exemple : PRIVMSG jreverdy :Are you a frog?
	std::vector<std::string> arg = splitString(str, " ");
	//if (arg.size() < 3)
	//{
	//	std::cerr << "ERR_NOSUCHNICK" << std::endl;
	//	return false;
	//}
	std::string nick_target = arg[1];
	//if (server->nickAlreadyExist(arg[2]) == true)
	//{
	//	std::cerr << "ERR_TOOMANYTARGETS" << std::endl;
	//	return false;
	//}
	//if (arg[2][0] != ':')
	//{
	//	std::cerr << "ERR_NOTEXTTOSEND" << std::endl;
	//	return false;
	//}
	//if (!server->nickAlreadyExist(nick_target))
	//{
	//	std::cerr << "ERR_NOSUCHNICK" << std::endl;
	//	return false;
	//}
    //for(unsigned int i = 0; i < arg.size(); i++)
    //{
    //    std::cout << i << " " << arg[i] << std::endl;
    //}
    User *dest = server->getUser(nick_target);
    std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + arg[1] + " " + arg[2] + "\r\n";
    send(dest->getSocket(), message.c_str(), message.size(), 0);
	return true;
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
            if (parsePass(user, str, server) == false)
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

        case 5:
            if (parseJoin(str, server, user) == false)
			{
				std::cerr << "Usage: JOIN <channel>" << std::endl;
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
            parseList(str, server);
            break;

        case 10:
             parseInvite(str, server, user);
             break;

        case 11:
            parseKick(str, server);
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

void    cmd::sendResponseToAllUsersInChannel(std::string message, Channel *channel)
{
    // std::cout << "Size of UserMap : " << channel->getUserList().size() << std::endl;
    // std::cout << "First user in map : " << &channel->getUserList().begin()->first<< std::endl;
    // std::cout << "First user's socket in map : " << channel->getUserList().begin()->first->getSocket() << std::endl;

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
        std::cout << "JOIN msg sent to " << user->first->getUsername() << std::endl;
        user++;
    }
    return ;
}