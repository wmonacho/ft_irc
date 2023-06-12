#include "cmd.hpp"

bool    cmd::parseJoin(std::string str, Server *server, User *user)
{
    // Parsing de la string (commande + argument )
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2) {
        // 461 ERR_NEEDMOREPARAMS
        std::string error = generateErrorMessage("461", splitArg[0]);
        send(user->getSocket(), error.c_str(), error.size(), 0);
	    return false;
    }
	std::vector<std::string> channels = splitString(splitArg[1], ",");
    if (splitArg[1][0] != '#' && splitArg[1][0] != '&')
    {
        // 475	ERR_BADCHANNELKEY
	    std::string error = generateErrorMessage("475", splitArg[0]);
        send(user->getSocket(), error.c_str(), error.size(), 0);
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

        // On envoie le topic s'il existe
        if (!channel->getTopic().empty()) {
            std::string topic = std::string(":localhost ") + "332" + " " + user->getUsername() + " #" + channel->getName() + " :" + channel->getTopic() + "\r\n";
            send(user->getSocket(), topic.c_str(), topic.size(), 0);
        }

        // liste des users
        if (channel->getUserList().size() > 0) {
            std::string user_list = std::string(":localhost ") + "353" + " " + user->getUsername() + " == #" + channel->getName() + " :";
            std::map<const User*, UserAspects> userMap = channel->getUserList();
            std::map<const User*, UserAspects>::iterator userNode = userMap.begin();
            std::map<const User*, UserAspects>::iterator lastUserNode = userMap.end();

            while (userNode != lastUserNode) {
                user_list.append(userNode->first->getUsername());
                if (userNode != userMap.end()--)
                    user_list.append(",");
                userNode++;
            }
            user_list.append("\r\n");

            // std::cout << "USER_LIST --> " << user_list;
            send(user->getSocket(), user_list.c_str(), user_list.size(), 0);
            std::string end_of_list = std::string(":localhost ") + "366" + " " + user->getUsername() + " #" + channel->getName() + ":End of NAMES list\r\n";
            send(user->getSocket(), end_of_list.c_str(), end_of_list.size(), 0);
        }
		return true;
	}

    // Cas 2 : le channel n'existe pas, il faut donc le creer dans notre serveur et y ajouter l'utilisateur
    UserAspects	new_aspects(1);
    Channel *channel = new Channel(channel_name);
    server->createNewChannel(channel_name, channel);
    std::cout << "DEBUG 1" << std::endl;
    if (!server->getChannel(channel_name))
	 return false;
    server->addUserToChannel(channel_name, user, new_aspects);
    sendMessageToAllUsersInChannel(server_response, server->getChannel(channel_name));
    std::cout << "DEBUG 2" << std::endl;
    return true;
}

//:localhost 353 user == #channel :user1 user2...
//:localhost 366 user #channel :End of NAMES list

// Users on :ebrodeur: issou
// Test cote serveur, on envoie bien ca :
    // :localhost 353 coucou == #lol :ebrodeur issou coucou
    
	// ERR_BANNEDFROMCHAN
	// ERR_INVITEONLYCHAN
	// ERR_CHANNELISFULL
	// ERR_BADCHANMASK
	// ERR_NOSUCHCHANNEL
	// ERR_TOOMANYCHANNELS