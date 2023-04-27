#include "server/server.hpp"
#include "client_srcs/weechat-docker/User.hpp"

int main(int ac, char **av)
{
	// User	inst;

	// (void)inst;
	Server server;

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

		/* This is where we will receive command, treat them and send the result to the client*/
		send(server.getNewSocket(), "Hello !\n", 8, 0);
		// Now we can read from the new_socket
		server.setValRead(read(server.getNewSocket(), buffer, 255));
		std::cout << "Message from the client : " << server.getValRead() << std::endl;
		// il faudra aussi recv

		// closing the connected socket
		close(server.getNewSocket());
		// closing the listening socket
		close(server.getSocketfd());
	
		return 0;
	}
	std::cout << "Usage : ./ircserv [PORT] [PASSWORD]" << std::endl;
	return 1;
}