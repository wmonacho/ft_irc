#include "server/server.hpp"
#include "user/User.hpp"
#include "cmd/cmd.hpp"

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

bool	checkPort(char *portString) {

	if (strlen(portString) != 4) {
		std::cerr << "Error: PORT should be 4 digits" << std::endl;
		return false;
	}
	for (size_t i = 0; i < strlen(portString); i++) {
		if (!isdigit(portString[i])) {
			std::cerr << "Error: PORT can only contains digits from 0 to 9" << std::endl;
			return false;
		}
	}
	int port = atoi(portString);
	// No need to check for value >9999 because we already checked that there is no more than 4 digits
	if (port < 1024) {
		std::cerr << "Error: wrong port value: PORT range is 1024 to 9999" << std::endl;
		return false;
	}
	return true;
}

int main(int ac, char **av)
{

	if (ac == 3)
	{
		// We check if the port has a correct value before configuring the server
		if (!checkPort(av[1]))
			return 1;

		Server server(atoi(av[1]), av[2]);

		server.startServer();

		// Closing the listening socket of the server
		close(server.getSocketfd());

		return 0;
	}
	std::cout << "Usage : ./ircserv [PORT] [PASSWORD]" << std::endl;
	return 1;
}