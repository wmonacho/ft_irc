#include "cmd.hpp"

bool    cmd::parseList(std::string str, Server *server, User *user)
{
    // Parsing user input
    std::vector<std::string> arg = splitString(str, " ");
    std::map<std::string, Channel*> map = server->getMap();
    std::vector<std::string> channels;

    if (map.empty()) {
        std::string list_start = std::string(":localhost ") + "321" + " " + arg[0] + "\r\n";
        std::string list_end = std::string(":localhost ") + "323" + " " + arg[0] + "\r\n";
        send(user->getSocket(), list_start.c_str(), list_start.size(), 0);
        send(user->getSocket(), list_end.c_str(), list_end.size(), 0);
    }

    if (arg.size() == 1)
    {
        std::string channels_list;
        for (std::map<std::string, Channel*>::iterator it = map.begin(); it != map.end(); it++)
        {
            channels_list.append(it->second->getName());
            channels_list.append(" ");
        }
        std::string list_message = std::string(":localhost ") + "321" + " " + arg[0] + "\r\n";
        send(user->getSocket(), list_message.c_str(), list_message.size(), 0);
        return true;
    }
    arg.erase(arg.begin());
    channels = splitString(arg[1], ",");
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

// /list = on liste tout les channels et leur topic
// /list #channel = on donne le status de ce channel (son topic etc)

// Private  channels  are  listed  (without  their topics)  as channel "Prv" unless the client generating the query is
// actually on that channel

// If there are no channels available to return, only the start and end reply must be sent.