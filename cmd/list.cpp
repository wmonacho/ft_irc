#include "cmd.hpp"

bool    cmd::parseList(std::string str, Server *server, User *user)
{
    // Parsing user input
    std::vector<std::string> arg = splitString(str, " ");
    std::map<std::string, Channel*> map = server->getMap();
    std::vector<std::string> chans;

    if (arg.size() == 1)
    {
        std::string channels_list;
        for (std::map<std::string, Channel*>::iterator it = map.begin(); it != map.end(); it++)
        {
            channels_list.append(it->second->getName());
            channels_list.append(" ");
        }
        std::string message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + channels_list + "\r\n";
        return true;
    }
    arg.erase(arg.begin());
    chans = splitString(arg[1], ",");
    for (std::vector<std::string>::iterator it = arg.begin(); it != arg.end(); it++)
    {
        std::map<std::string, Channel*>::iterator itMap = map.begin();
        int isValid = 0;
        while (itMap != map.end())
        {
            if (itMap->second->getName() == *it)
            {
                std::cout << "Channel: " << itMap->second->getName() << std::endl;
                std::cout << " - topic: " << itMap->second->getTopic() << std::endl;
                isValid = 1;
            }
            else
                itMap++;
        }
        if (isValid == 0)
        {
            std::cerr << "Channel: " << itMap->second->getName() << "doesn't exist."  << std::endl;
            return false;
        }
    }
    return true;
}