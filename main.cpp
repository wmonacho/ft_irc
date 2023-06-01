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
		while (i < 5)
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
			std::cout << user2.getNickname() << std::endl;
			std::cout << server.getChannel("channel")->getUser(&user2)->getNickname() << std::endl;
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