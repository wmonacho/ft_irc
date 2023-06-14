#include "cmd.hpp"

bool    cmd::parseMode(std::string str, Server *server, User *user)
{
    //mode i, t, k, o, l
    std::vector<std::string> splitArg = splitString(str, " ");
	//MODE <cible> <mode> <argument(s)>
	if (splitArg.size() == 2)
	{
		// MODE <channel>
		//envoie des modes du channel? de quel maniere l'ecrire? 
		;
	}
    if (splitArg.size() < 3)
    {
        // 461  ERR_NEEDMOREPARAMS
        std::string error = std::string(":localhost ") + "461 " + user->getNickname() + " " + splitArg[0] + " :Not enough parameters" + "\r\n";
		send(user->getSocket(), error.c_str(), error.size(), 0);
        return (false);
    }
    //check si # devant la cible et si le channel existe
	if ((splitArg[1][0] != '#' && splitArg[1][0] != '&') || server->getMap().find(&splitArg[1][1]) == server->getMap().end())
	{
           return (false);
	}
	//check si le User est bien dans la userlist du channel
	if (!server->userInChannel(&splitArg[1][1], user))
	{
			// 441 ERR_USERNOTINCHANNEL
			std::string error = std::string(":localhost ") + "441 " + user->getNickname() + " " + &splitArg[1][1] + " :They aren't on that channel" + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
        	return false;
	}
    //check si + ou - devant le mode
	if ((splitArg[2][0] != '-' && splitArg[2][0] != '+') || splitArg[2].size() != 2)
	{
			// 477 ERR_NOCHANMODES
        	std::string error = std::string(":localhost ") + "477 " + user->getNickname() + " " +  &splitArg[1][1] + " :Channel doesn't support modes" + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
        	return (false);
	}
    //check si le mode existe (tout depend de ceux que l'on prend)
	std::string modes = "iktlo";
	if (modes.find(&splitArg[2][1]) == std::string::npos)
	{
			// 472 ERR_UNKNOWNMODE
			std::string error = std::string(":localhost ") + "472 " + user->getNickname() + " " +  &splitArg[2][1] + " :is unknown mode char to me for " + &splitArg[1][1] + "\r\n";
			send(user->getSocket(), error.c_str(), error.size(), 0);
			return (false);
	}
    Channel *chan = server->getChannel(&splitArg[1][1]);
    const User* u = server->getChannelUser(&splitArg[1][1], splitArg[3]);
    //execute les modes +
    //il restera a modifier les fonctions affectees par les modes

	for (unsigned int i = 1; splitArg[2][0] == '+' && i < splitArg[2].size(); i++)
	{
		const User* u = server->getChannelUser(&splitArg[1][1], splitArg[3]);
		std::string rpl_channel_mode_is = std::string(":localhost ") + "467 " + user->getNickname() + " " + splitArg[1] + " +" + splitArg[2][i] + "\r\n";
		switch(splitArg[2][i])
		{
                  case 105:
                    	//execute mode i
                    	chan->setInviteOnly(true);
						// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                    	break;
                  case 107:
                    	//execute mode k
						if (chan->getPassword() != "") {
							// 467 ERR_KEYSET
							std::string error = std::string(":localhost ") + "467 " + user->getNickname() + " " + splitArg[1] + " :Channel key already set" + "\r\n";
							send(user->getSocket(), error.c_str(), error.size(), 0);
							return (false);
						}
                    	chan->setPassword(splitArg[3]);
						// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                    	break;
                  case 108:
                    	chan->setUserLimit(atoi(splitArg[3].c_str()));
						// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                    	break;
                      	//execute mode L
                  case 111:
                      	//execute mode o
						chan->changeUserAdmin(u, true);
						// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      	break;
                  case 116:
                      	//execute mode t
                      	chan->setTopicAdmin(true);
						// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      	break;
		}
	}
	//execute les modes -
	for (unsigned int i = 1; splitArg[2][0] == '-' && i < splitArg[2].size(); i++)
	{
		std::string rpl_channel_mode_is = std::string(":localhost ") + "467 " + user->getNickname() + " " + splitArg[1] + " -" + splitArg[2][i] + "\r\n";
		switch(splitArg[2][i])
		{
                  case 105:
                      	//execute mode i
                      	chan->setInviteOnly(false);
					  	// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      break;
                  case 107:
                      	//execute mode k
                      	chan->setPassword("");
					  	// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      break;
                  case 108:
                      	//execute mode L
                      	chan->setUserLimit(-1);
					  	// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      break;
                  case 111:
                      	//execute mode o
                      	chan->changeUserAdmin(u, false);
					  	// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      break;
                  case 116:
                      	chan->setTopicAdmin(false);
					  	// 324 RPL_CHANNELMODEIS
						send(user->getSocket(), rpl_channel_mode_is.c_str(), rpl_channel_mode_is.size(), 0);
                      break;
		}
	}
	return (true);
}

// Numeric Replies:

//
//                           ERR_CHANOPRIVSNEEDED a voir
//                       
//          346 RPL_INVITELIST              347    RPL_ENDOFINVITELIST
//           RPL_UNIQOPIS
