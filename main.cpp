#include "server/server.hpp"
#include "user/User.hpp"
#include "cmd/cmd.hpp"

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

int main(int ac, char **av)
{

	if (ac == 3)
	{
		Server server(atoi(av[1]), av[2]);
    
		server.startServer();
		//// closing the connected socket
		close(server.getNewSocket());
		//// closing the listening socket
		close(server.getSocketfd());

		/*Server server;
		std::string line;
		cmd			cmd;
		User		user1;
		User		user2;
		int			i = 0;
		user1.setNickname("will");
		user1.setUsername("will_");
		user1.setRealname("will mona");
		user2.setNickname("doud");
		user2.setUsername("doud_");
		user2.setRealname("doud mona");
		server.setUserList(user1);
		server.setUserList(user2);
		(void)av;
		while (i < 1)
		{
			std::cout << "Enter user1 CMD :";
			std::getline(std::cin, line);
			cmd.whichCmd(line, &server, server.getUser("will"));
			i++;
		}
		while (true)
		{
			std::cout << "Enter user2 CMD :";
			std::getline(std::cin, line);
			cmd.whichCmd(line, &server, server.getUser("doud"));
		}*/
		return 0;
	}
	std::cout << "Usage : ./ircserv [PORT] [PASSWORD]" << std::endl;
	return 1;
}