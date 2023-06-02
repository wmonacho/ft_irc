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
	if (ac == 3)
	{
		Server server(atoi(av[1]), av[2]);

		server.startServer();

		// closing the connected socket
		close(server.getNewSocket());
		// closing the listening socket
		close(server.getSocketfd());
	
		return 0;
	}
	std::cout << "Usage : ./ircserv [PORT] [PASSWORD]" << std::endl;
	return 1;
}

// In serverStart :
	// je creer un socketPool (vector ou array de pollfd?)