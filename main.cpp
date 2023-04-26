#include "server.hpp"

int main(int ac, char **av)
{
	//Server server;
	(void)ac;

	// Il faudra passer ca dans la class Server et penser a modifier le server.cpp en consequence
	int socketfd;
	int	new_socket;
	int	valread;
	int	port;
	struct sockaddr_in servAddr, clientAddr;
	socklen_t servLen, clientLen;
	servLen = sizeof(servAddr);
	clientLen = sizeof(clientAddr);
	char buffer[256];

	// We create a socket that can communicate with IPv4 (AddressFamily_INET)
	if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "Error : socket creation failed" << std::endl;
		exit(1);
	}

	port = atoi(av[1]);

	/* sockaddr_in structure setup before bind() call */
	// Server byte order
	servAddr.sin_family = AF_INET;
	// Fill with current host's IP address
	servAddr.sin_addr.s_addr = INADDR_ANY;
	// The htons() function converts the unsigned short integer hostshort from host byte order to network byte order
	servAddr.sin_port = htons(port);

	// Once we have a socket, we have to associate that socket with a port on our local machine
	// This bind() call will bind the socket to the current IP address on port
	if (bind(socketfd, (struct sockaddr*)&servAddr, servLen) < 0) {
		std::cout << "Error : socket binding failed" << std::endl;
		exit(1);
	}

	// Then we wait for the client to approach the server for connection (passive mode)
	// the last argument (3 here) is the maximum size for the backlog queue
	if (listen(socketfd, 3) < 0) {
		std::cout << "Error : listen failed" << std::endl;
		exit(1);
	}

	// Finally we extract the first connection request from the queue of pending connections for the listening socket
	// then we create a new connected socket and we return a new fd referring to that socket
	if ((new_socket = accept(socketfd, (struct sockaddr*)&clientAddr, &clientLen)) < 0) {
		std::cout << "Error : new socket creation failed" << std::endl;
		exit(1);
	}

	std::cout << "Server got a connection from " << inet_ntoa(clientAddr.sin_addr) << " on port " << ntohs(clientAddr.sin_port) << std::endl;

	/* This is where we will receive command, treat them and send the result to the client*/
	send(new_socket, "Hello !\n", 8, 0);
	// Now we can read from the new_socket
	valread = read(new_socket, buffer, 255);
	std::cout << "Message from the client : " << valread << std::endl;
	// il faudra aussi recv

	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	close(socketfd);

	return 0;
}