#include "cmd.hpp"

bool	cmd::parseNames(std::string str, Server *server, User *user)
{
	std::vector<std::string> arg = splitString(str, " ");

	std::map<std::string, Channel*> map = server->getMap();
	std::vector<User> userListCopy = server->getUserList();

	if (arg.size() == 1)
	{
	 std::cout << "frome one" << std::endl;
	 for (std::map<std::string, Channel*>::iterator it = map.begin(); it != map.end(); it++)
		{
		 std::cout << "Channel: " << it->first << std::endl;
			std::map<const User*, UserAspects> userlist = it->second->getUserList();
			for (std::map<const User*, UserAspects>::iterator itUser = userlist.begin(); itUser != userlist.end(); itUser++)
			{
				std::cout << itUser->first->getUsername() << std::endl;
				std::vector<User>::iterator iter = std::find(userListCopy.begin(), userListCopy.end(), itUser->first);
		  		std::string rpl_namreply = std::string(":localhost ") + "353" + " " + itUser->first->getNickname();
		  		send(user->getSocket(), rpl_namreply.c_str(), rpl_namreply.size(), 0);
				if (iter != userListCopy.end())
					userListCopy.erase(iter);
		 }
		 std::string rpl_endofnames = std::string(":localhost ") + "366" + " " + it->first + ":End of NAMES list\r\n";
		 send(user->getSocket(), rpl_endofnames.c_str(), rpl_endofnames.size(), 0);
	 }
		for (std::vector<User>::iterator cpyIt = userListCopy.begin(); cpyIt != userListCopy.end(); cpyIt++)
			std::cout << "'*' " << (*cpyIt).getUsername() << std::endl;
	}
	if (arg.size() == 2)
	{
	 std::string tmp = arg[1];
	 std::vector<std::string> chanArg = splitString(tmp, ",");
	 std::cout << "here" << std::endl;
	 for (long unsigned int i = 0; i < arg.size(); i++)
	 {
		 Channel	*chan = server->getChannel(arg[i]);
		 std::cout << chanArg[i] << std::endl;
		 if (server->channelAlreadyExist(arg[i]) == true)
		 {
		  std::cout << "Channel: " << chanArg[i] << " ";
		  std::map<const User*, UserAspects> userlist = chan->getUserList();
		  for (std::map<const User*, UserAspects>::iterator itUser = userlist.begin(); itUser != userlist.end(); itUser++)
		  {
			  std::cout << itUser->first->getUsername() << std::endl;
			  std::string rpl_namreply = std::string(":localhost ") + "353" + " " + itUser->first->getUsername();
			  send(user->getSocket(), rpl_namreply.c_str(), rpl_namreply.size(), 0);
		  }
		  std::string rpl_endofnames = std::string(":localhost ") + "366" + " " + arg[i] + ":End of NAMES list\r\n";
		  send(user->getSocket(), rpl_endofnames.c_str(), rpl_endofnames.size(), 0);
		 }
	 }
		//print les channels passes en parametre ainsi que leurs users respectifs tout en faisant attention
		//a ne pas afficher les utilisateurs qui ne sont pas visibles.
	}
	return true;
}