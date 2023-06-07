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

    // Cas 1 : on renvoie le topic
    if (arg.size() == 2) {
        std::string topic_message = ":localhost " + arg[0] + " " + channel->getTopic() + "\r\n";
        std::cout << "TOPIC : " << topic_message << std::endl;
        send(user->getSocket(), topic_message.c_str(), topic_message.size(), 0);
        return true;
    }
    
    // Cas 2 : on set le nouveau topic
    if (arg.size() == 3) {
        std::string new_topic = arg[2];
        new_topic.erase(0, 1);
        std::cout << "Setting up new topic --> " << new_topic << std::endl;
        channel->setTopic(new_topic);
        return true ;
    }
    return true;
}