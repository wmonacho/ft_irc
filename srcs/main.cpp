#include "server/server.hpp"
#include "client_srcs/User.hpp"

int main(int ac, char **av)
{
	// User	inst;

	// (void)inst;
	Server server;
	struct pollfd fds;

	if (ac == 3)
	{

		Server server;
		char buffer[256]; // for testing purpose


		// We create a socket that can communicate with IPv4 (AddressFamily_INET)
		server.setSocketfd(socket(AF_INET, SOCK_STREAM, 0));
		if (server.getSocketfd() < 0) {
			std::cout << "Error : socket creation failed" << std::endl;
			exit(1);
		}

		server.setPort(atoi(av[1]));

		/* sockaddr_in structure setup before bind() call */
		server.setServAddr(server.getPort());

		// Once we have a socket, we have to associate that socket with a port on our local machine
		// This bind() call will bind the socket to the current IP address on port
		if (bind(server.getSocketfd(), (struct sockaddr*)server.getServAddr(), server.getServLen()) < 0) {
			std::cout << "Error : socket binding failed" << std::endl;
			exit(1);
		}

		// Then we wait for the client to approach the server for connection (passive mode)
		// the last argument (3 here) is the maximum size for the backlog queue
		if (listen(server.getSocketfd(), 3) < 0) {
			std::cout << "Error : listen failed" << std::endl;
			exit(1);
		}

		// Finally we extract the first connection request from the queue of pending connections for the listening socket
		// then we create a new connected socket and we return a new fd referring to that socket
		server.setNewSocket(accept(server.getSocketfd(), (struct sockaddr*)server.getClientAddr(), server.getClientLen()));
		if (server.getNewSocket() < 0) {
			std::cout << "Error : new socket creation failed" << std::endl;
			exit(1);
		}

		std::cout << "Server got a connection from " << inet_ntoa(server.getClientAddr()->sin_addr) << " on port " << ntohs(server.getClientAddr()->sin_port) << std::endl;

		fds.fd = server.getNewSocket();
		fds.events = POLLIN;

		while (1) {
			int num_events = poll(&fds, 1, -1); // Wait indefinitely for events
			if (num_events == -1) {
				// Handle poll error
				std::cout << "Poll error" << std::endl;
				close(server.getNewSocket());
				close(server.getSocketfd());
				exit(1);
			}
			else if (num_events == 0) {
				// No events occurred before timeout
				std::cout << "RAS je sais pas encore quoi faire ici" << std::endl;
			}
			else {
				// Check if the client socket has data available for reading
				if (fds.revents & POLLIN) {
					// Handle incoming messages from the client
					memset(buffer, 0, 255);
					if (recv(server.getNewSocket(), buffer, 255, 0) > 0)
						std::cout << "Message from the client : " << buffer;
				}
			}
		}
		// closing the connected socket
		close(server.getNewSocket());
		// closing the listening socket
		close(server.getSocketfd());
	
		return 0;
	}
	std::cout << "Usage : ./ircserv [PORT] [PASSWORD]" << std::endl;
	return 1;
}