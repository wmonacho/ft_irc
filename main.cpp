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
		std::cout << "Entrez une ligne : ";
		user.setNickname("will");
		user.setUsername("will_");
		user.setRealname("will mona");
		server.setUserList(user);
		std::string line;
		int i = 0;
		while (i < 1)
		{
			std::cout << "DEBUG" << std::endl;
			std::getline(std::cin, line);
			if (std::cin.eof())
                exit(1);
			cmd.whichCmd(line, &server, &user);
			std::cout << user.getNickname() << std::endl;
			i++;
		}
		User	user2;
		std::cout << "Entrez une ligne : ";
		user2.setNickname("doudr");
		user2.setUsername("doudr_");
		user2.setRealname("doudr mona");
		server.setUserList(user2);
		while (true)
		{
			std::cout << "DEBUG2" << std::endl;
			std::getline(std::cin, line);
			if (std::cin.eof())
                exit(1);
			cmd.whichCmd(line, &server, &user2);
			//envoie de JOIN #channel et de NICK didou puis de KICK willou
			std::cout << "user2_addr b :" << &user2 << std::endl;
			std::cout << "user2_addr a :" << &(*server.getChannelUser("channel", &user2)) << std::endl;
			std::cout << "user2 nickname b :" << user2.getNickname() << ":" << std::endl;
			std::cout << "user2 nickname a :" << server.getChannelUser("channel", &user2)->getNickname() << ":" << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << " " << std::endl;
			std::cout << "====================================================" << std::endl;
			std::cout << "user_addr b :" << &user << std::endl;
			std::cout << "user_addr a :" << &(*server.getChannelUser("channel", &user)) << std::endl;
			std::cout << "user nickname b :" << user.getNickname() <<  ":" << std::endl;
			std::cout << "user nickname a :" << server.getChannelUser("channel", &user)->getNickname() << ":" << std::endl;
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