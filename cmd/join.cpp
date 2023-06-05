#include "cmd.hpp"

bool    cmd::parseJoin(std::string str, Server *server, User *user)
{
    // std::cout << "str: " << str << std::endl;
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2)
	 return false;
    //check si pas de # devant le channel (jwe crois que l'on peut mettre & aussi a verifier)
    if (splitArg[1][0] != '#')
    {
	 std::cerr << "ERR_BADCHANNELKEY" << std::endl;
	 return false;
    }
    std::string channel_name= &splitArg[1][1];
    std::string server_response = createServerMessage(user, "", splitArg);
    if (server->channelAlreadyExist(channel_name))
	{
	    Channel* channel = server->getChannel(channel_name);
        std::cout << "Joining --> " << channel->getName() << std::endl;
 		UserAspects	new_aspects(0);
		// channel->setUserList(user, new_aspects);
        server->addUserToChannel(channel_name, user, new_aspects);
        // We send a message to all the users connected to the channel
        sendResponseToAllUsersInChannel(server_response, channel);
        sendChannelTopicToUser(channel, user);
		return true;
	}
    /*snon creer un nouveau Channel y ajouter le User avec les droits admin et utiliser setNewChannelInMap ensuite*/
    UserAspects	new_aspects(1);
    Channel *channel = new Channel(channel_name);
    server->createNewChannel(channel_name, channel);
    if (!server->getChannel(channel_name))
	 return false;
    server->addUserToChannel(channel_name, user, new_aspects);
    // After the channel creation (if it didn't exist)
    sendResponseToAllUsersInChannel(server_response, server->getChannel(channel_name));
    return true;
}

void    cmd::sendChannelTopicToUser(Channel *channel, User *user) {

    std::string topic_message = std::string(":localhost") + " " + "332" + " " + user->getUsername() + " " + channel->getName() + " " + channel->getTopic() + "\r\n";
    std::cout << "TOPIC RETURN => " << topic_message << std::endl;
    send(user->getSocket(), topic_message.c_str(), topic_message.size(), 0);
    return ;
}