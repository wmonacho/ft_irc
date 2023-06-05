#include "cmd.hpp"

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