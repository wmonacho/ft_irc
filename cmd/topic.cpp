#include "cmd.hpp"

bool    cmd::parseTopic(std::string str, Server *server, User *user)
{
    std::vector<std::string> arg = splitString(str, " ");
    std::cout << "ARG size ==> " << arg.size() << std::endl;
    if (arg.size() < 2)
    {
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return false;
    }

    Channel *channel = server->getChannel(&arg[1][1]);
    if (!channel) {
        std::cerr << "Error: Channel not found" << std::endl;
        return false;
    }

    // Cas 1 : on renvoie le topic s'il existe
    if (arg.size() == 2 && (!channel->getTopic().empty())) {
        std::string topic_message = std::string(":localhost ") + "332" + " " + user->getUsername() + " #" + channel->getName() + " :" + channel->getTopic() + "\r\n";
        std::cout << "TOPIC : " << topic_message << std::endl;
        send(user->getSocket(), topic_message.c_str(), topic_message.size(), 0);
        return true;
    }
    
    // Cas 2 : on set le nouveau topic
    if (arg.size() >= 3) {
        rebuildMessage(arg, 2);
        std::string new_topic = arg[2];
        new_topic.erase(0, 2);
        std::cout << "Setting up new topic --> " << new_topic << std::endl;
        channel->setTopic(new_topic);
        return true ;
    }
    return true;
}