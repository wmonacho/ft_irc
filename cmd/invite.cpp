#include "cmd.hpp"

bool    cmd::parseInvite(std::string str, Server *server, User *user)
{
    std::vector<std::string> arg = splitString(str, " ");

    if (arg.size() != 3)
        return false;
    std::string nick = arg[1];
    std::string chan = &arg[2][1];
    std::map<std::string, Channel*> map = server->getMap();
    int chanFound = 0;
    for (std::map<std::string, Channel*>::iterator it = map.begin(); it != map.end(); it++)
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