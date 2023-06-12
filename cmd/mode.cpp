#include "cmd.hpp"

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
	if ((splitArg[1][0] != '#' && splitArg[1][0] != '&') || server->getMap().find(&splitArg[1][1]) == server->getMap().end())
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