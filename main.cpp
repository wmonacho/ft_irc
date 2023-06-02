#include "server/server.hpp"
#include "user/User.hpp"
#include "cmd/cmd.hpp"

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

int main(int ac, char **av)
{
// CAP LS 302
// NICK ebrodeur
// USER ebrodeur 0 * :realname
	(void)av;
	if (ac != 10)
	{
		cmd	cmd;
		Server server;
		User	user;
		user.setNickname("will");
		user.setUsername("will_");
		user.setRealname("will mona");
		server.setUserList(user);
		std::string line;
		int i = 0;
		while (i < 1)
		{
			std::getline(std::cin, line);
			if (std::cin.eof())
                exit(1);
			cmd.whichCmd(line, &server, server.getUser("will"));
			Channel *channel = server.getChannel("channel");
			std::cout << "channel_addr :" << &(*channel) << std::endl;
			if (server.channelAlreadyExist("channel"))
				std::cout << "channel exist" << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << "will_addr server :" << &(*server.getUser("will")) << std::endl;
			std::cout << "will_addr channel :" << &(*channel->getUser(&user)) << std::endl;
			std::cout << "will nickname server :" << server.getUser("will")->getNickname() <<  ":" << std::endl;
			//std::cout << "will nickname channel :" << server.getChannelUser("channel", &user)->getNickname() << ":" << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << " " << std::endl;
			i++;
		}
		User	user2;
		user2.setNickname("doudr");
		user2.setUsername("doudr_");
		user2.setRealname("doudr mona");
		server.setUserList(user2);
		while (true)
		{
			std::getline(std::cin, line);
			if (std::cin.eof())
                exit(1);
			cmd.whichCmd(line, &server, server.getUser("doudr"));
			//envoie de JOIN #channel puis KICK will
			std::cout << "====================================================" << std::endl;
			std::cout << "will_addr server :" << &(*server.getUser("will")) << std::endl;
			std::cout << "will_addr channel :" << &(*server.getChannelUser("channel", &user)) << std::endl;
			std::cout << "will nickname server :" << server.getUser("will")->getNickname() <<  ":" << std::endl;
			//std::cout << "will nickname channel :" << server.getChannelUser("channel", &user)->getNickname() << ":" << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << " " << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << "doudr_addr server :" << &(*server.getUser("doudr")) << std::endl;
			std::cout << "doudr_addr channel :" << &(*server.getChannelUser("channel", &user2)) << std::endl;
			std::cout << "doudr nickname server :" << server.getUser("doudr")->getNickname() << ":" << std::endl;
			//std::cout << "doudr nickname channel :" << server.getChannelUser("channel", &user2)->getNickname() << ":" << std::endl;
			std::cout << "====================================================" << std::endl;
		}
		//server.startServer();
		
		// closing the connected socket
		//close(server.getNewSocket());
		// closing the listening socket
		//close(server.getSocketfd());
	
		return 0;
	}
	std::cout << "Usage : ./ircserv [PORT] [PASSWORD]" << std::endl;
	return 1;
}

// In serverStart :
	// je creer un socketPool (vector ou array de pollfd?)