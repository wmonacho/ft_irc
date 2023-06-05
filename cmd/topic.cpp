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
    //verifier si le channel existe
    // if (arg[1][0] == '#' || !server->channelAlreadyExist(&arg[1][1]))
    //     return false;
    // //verifier si le client est dans le channel
    // if (!server->userInChannel(arg[1], user))
    //     return false;

    // si TOPIC channel --> on return le topic du channel
    std::cout << str << std::endl;
    Channel *channel = server->getChannel(arg[1]);
    if (arg.size() == 2) {
        std::string topic = channel->getTopic();
        std::string topic_message = ":" + user->getNickname() + "!" + user->getUsername() + "@locahost " + arg[0] + " " + topic + "\r\n";
        std::cout << "TOPIC : " << topic_message << std::endl;
        send(user->getSocket(), topic_message.c_str(), topic_message.size(), 0);
        return true;
    }
    
    // si TOPIC channel text --> on set le topic du channel avec text
    if (arg.size() == 3) {
        std::cout << "Setting up new topic" << std::endl;
        std::string new_topic = arg[2];
        std::cout << new_topic << std::endl;
        channel->setTopic(new_topic); //segfault
        return true ;
    }

    // if (arg.size() == 2)
    // {
    //     if (chan->getTopicAdmin())
    //     {
    //         if (!chan->getUserAdmin(user))
    //         {
    //             std::cerr << "User has to be admin to edit topic" << std::endl;
    //             return false;
    //         }
    //         else
    //         {
    //             chan->setTopic(arg[3]);
    //             return true;
    //         }
    //     }
    //     if (arg[2] == "")
    //     {
    //         chan->setTopic("");
    //         return true;
    //     }
    //     else
    //     {
    //         chan->setTopic(arg[2]);
    //         return true;
    //     }
    // }
    return true;
}