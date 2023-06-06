#include "cmd.hpp"

bool    cmd::parseJoin(std::string str, Server *server, User *user)
{
    // Parsing de la string (commande + argument )
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2)
	 return false;
    if (splitArg[1][0] != '#')
    {
	 std::cerr << "ERR_BADCHANNELKEY" << std::endl;
	 return false;
    }
    std::string channel_name= &splitArg[1][1];
    std::string server_response = createServerMessage(user, "", splitArg);

    // Cas 1 : le channel existe, donc y ajoute le user et on envoie le message de "bienvenue" a tout le monde
    // + on envoie le topic (s'il existe) et la liste des USER 
    if (server->channelAlreadyExist(channel_name))
	{
	    Channel* channel = server->getChannel(channel_name);
 		UserAspects	new_aspects(0);
        server->addUserToChannel(channel_name, user, new_aspects);
        sendMessageToAllUsersInChannel(server_response, channel);
        // topic s'il existe
        // liste des users
        if (channel->getUserList().size() > 1) {
            std::string user_list = std::string(":localhost ") + "353" + " " + user->getUsername() + " == #" + channel->getName() + ":";
            std::map<const User*, UserAspects> userMap = channel->getUserList();
            std::map<const User*, UserAspects>::iterator userNode = userMap.begin();
            std::map<const User*, UserAspects>::iterator lastUserNode = userMap.end();

            while (userNode != lastUserNode) {
                user_list.append(userNode->first->getUsername());
                user_list.append(" ");
                userNode++;
            }
            user_list.append("\r\n");
            std::cout << "USER_LIST --> " << user_list;
            send(user->getSocket(), user_list.c_str(), user_list.size(), 0);
        }
		return true;
	}

    // Cas 2 : le channel n'existe pas, il faut donc le creer dans notre serveur et y ajouter l'utilisateur
    UserAspects	new_aspects(1);
    Channel *channel = new Channel(channel_name);
    server->createNewChannel(channel_name, channel);
    if (!server->getChannel(channel_name))
	 return false;
    server->addUserToChannel(channel_name, user, new_aspects);
    sendMessageToAllUsersInChannel(server_response, server->getChannel(channel_name));
    //sendChannelTopicToUser(server->getChannel(channel_name), user);
    return true;
}

void    cmd::sendChannelTopicToUser(Channel *channel, User *user) {

    std::string topic_message = std::string(":localhost") + " " + "332" + " " + user->getUsername() + " " + channel->getName() + " " + channel->getTopic() + "\r\n";
    std::cout << "TOPIC RETURN => " << topic_message << std::endl;
    send(user->getSocket(), topic_message.c_str(), topic_message.size(), 0);
    return ;
}