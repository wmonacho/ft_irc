#include "server.hpp"
#include <cerrno>
#include <string.h>
#include <fcntl.h>

void    Server::startServer() {

	int             connectionStatus, pollReturn, socketID;
	int             nfds = 1;
	int             currentSize = 0;
	bool            closeConnection = false;
	bool            endOfServer = false;
	struct pollfd   fds[MAX_SOCKETS];
	
	// Initialize the array of pollfd structures
	memset(fds, 0, sizeof(fds));

	// Set the first struct of the array to the listening socket
	fds[0].fd = this->_socketfd;
	fds[0].events = POLLIN;

	// The main loops implements poll() : we detect if the socket is connecting or connected and act in consequence
	do {
		pollReturn = poll(fds, nfds, -1);
		if (pollReturn < 0) {
			std::cerr << "Error: poll() failed" << std::endl;
			break ;
		}
		if (pollReturn == 0) {
			std::cerr << "Error: poll() timed out" << std::endl;
			break ;
		}
		// We set the currentSize to the number of sockets in our pollfd array
		currentSize = nfds;
		for (socketID = 0; socketID < currentSize; socketID++) {
			// If it's a connecting socket we accept the connection and add it to the socket pool (fds[nfds])
			if (fds[socketID].fd == this->_socketfd) {
				connectionStatus = acceptNewConnection(fds, nfds);
				if (connectionStatus == -1)
					endOfServer = true;
				else
					nfds = connectionStatus; // connectionStatus is the number of fd if acceptNewConnection() succeeded
			}
			// If it's already a connected socket, we try to receive the data
			else {
				closeConnection = retrieveDataFromConnectedSocket(socketID, fds, closeConnection);
				if (closeConnection) {
					std::cerr << "Closing connection for socket " << socketID << std::endl;
					close(fds[socketID].fd);
					fds[socketID].fd = -1;
				}
			}
		}
	} while (endOfServer == false);

	// If the server ended we should close all fd
	for (int i = 0; i < nfds; i++) {
		if (fds[i].fd >= 0) {
			std::cerr << "Closing socket " << i << std::endl;
			close(fds[i].fd);
		}
	}

	return ;
}

/*******************************/
/* CONNECTING SOCKET FUNCTIONS */
/*******************************/

int Server::acceptNewConnection(struct pollfd *fds, int nfds) {

	int newSocket;
	int errorStatus = -1;
	int	error;

	do {
		newSocket = accept(this->_socketfd, NULL, NULL);
		if (newSocket < 0) { 
			if (errno != EWOULDBLOCK) {
				std::cerr << "Error: accept() failed" << std::endl;
				nfds = errorStatus;
			}
			std::cout << "Socket " << nfds << " is already connected" << std::endl;
			break ;
		}
		fds[nfds].fd = newSocket;
		fds[nfds].events = POLLIN;
		error = verifyClientAndServerResponse(fds[nfds]);
		if (error == 1)
			return (-1);
		if (error == 2) {
			std::string errorSameUsername = "Error: this username is already used, try a new one\r\n";
			send(fds[nfds].fd, errorSameUsername.c_str(), errorSameUsername.size(), 0);
			return nfds;
		}
		nfds++;
	} while (newSocket != -1);
	return (nfds);
}

int Server::verifyClientAndServerResponse(struct pollfd fds) {

	// We retrieve the informations sent by the client to create the new user which connected to our server
	// and we send back the RPL_WELCOME

	Server::userConnectionRegistration *userInfo = getUserConnectionRegistrationStruct();
	std::string server_response_for_connection;

	// We retrieve the data from the socket in buffer
	if (getClientInformationsOnConnection(fds, userInfo) == false) {
		std::cerr << "Error: user information on connection could't be handled" << std::endl;
		return (1);
	}

	// This function parse the buffer to find the username and nickname of the user who connected to the server
	// and it creates a new user in the server's users_list
	server_response_for_connection = createServerResponseForConnection(fds.fd, userInfo);
	if (server_response_for_connection.empty())
		return (2);

	// Finally we send back the server response to confirm the connection of the user
	send(fds.fd, server_response_for_connection.c_str(), server_response_for_connection.size(), 0);
	return (0);
}

bool	Server::findPassInBuffer(char *buffer, Server::userConnectionRegistration *userInfo) {

	char	pass[] = "PASS";

	for (size_t i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == 'P') {
			if (strncmp(pass, &buffer[i], 4) == 0) {
				int j = i;
				while (buffer[j] != '\r')
					j++;
				std::string tmp(&buffer[i], j - i);
				size_t passPos = tmp.find("PASS");
				passPos += 5;
				std::string pwd = tmp.substr(passPos);
				size_t passLimiter = pwd.find("\r\n");
				pwd = pwd.substr(0, passLimiter);
				userInfo->password = pwd;
				return true;
			}
		}
	}
	return false;
}

bool	Server::findNickInBuffer(char *buffer, Server::userConnectionRegistration *userInfo) {

	char	nick[] = "NICK";

	for (size_t i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == 'N') {
			if (strncmp(nick, &buffer[i], 4) == 0) {
				int j = i;
				while (buffer[j] != '\r')
					j++;
				std::string tmp(&buffer[i], j - i);
				size_t nickPos = tmp.find("NICK");
				nickPos += 5;
				std::string nick = tmp.substr(nickPos);
				size_t passLimiter = nick.find("\r\n");
				nick = nick.substr(0, passLimiter);
				userInfo->nickName = nick;
				return true;
			}
		}
	}
	return false;
}

bool	Server::findUserInBuffer(char *buffer, Server::userConnectionRegistration *userInfo) {

	char	user[] = "USER";

	for (size_t i = 0; i < strlen(buffer); i++) {
		if (buffer[i] == 'U') {
			if (strncmp(user, &buffer[i], 4) == 0) {
				int j = i;
				while (buffer[j] != '\r')
					j++;
				std::string tmp(&buffer[i], j - i);
				size_t userPos = tmp.find("USER");
				userPos += 5;
				std::string user = tmp.substr(userPos);
				size_t passLimiter = user.find(" ");
				user = user.substr(0, passLimiter);
				userInfo->userName = user;
				return true;
			}
		}
	}
	return false;
}

bool Server::getClientInformationsOnConnection(struct pollfd fds, Server::userConnectionRegistration *userInfo) {

	bool	passCheck = false;
	bool	nickCheck = false;
	bool	userCheck = false;
	int		bufferSize = 512;
	char	buffer[bufferSize];

	memset(buffer, 0, sizeof(char) * (bufferSize - 1));

	while (1) {
		int bytesRead = recv(fds.fd, buffer, bufferSize, 0);
		if (bytesRead == 0) {
			std::cerr << "Error: connection closed" << std::endl;
			return false;
		}
		else if (bytesRead < 0) {
			std::cerr << "Error: recv() failed" << std::endl;
			return false;
		}
		else {
			if (passCheck == false)
				passCheck = findPassInBuffer(buffer, userInfo);
			if (nickCheck == false)
				nickCheck = findNickInBuffer(buffer, userInfo);
			if (userCheck == false)
				userCheck = findUserInBuffer(buffer, userInfo);
			std::cout << "DEBUG PC ==> " << passCheck << std::endl;
			std::cout << "DEBUG NC ==> " << nickCheck << std::endl;
			std::cout << "DEBUG UC ==> " << userCheck << std::endl;
			if (passCheck == true && nickCheck == true && userCheck == true) {
				std::cout << "== ALL CONNECTION INFORMATIONS RETRIEVED ==" << std::endl;
				break ;
			}
		}
	}

	return true;
}

void    Server::createNewUserAtConnection(std::string nickname, std::string username, int socket) {

	// We create a new user and set his nickname and realname thanks to the message the client sent
	User new_user;

    new_user.setNickname(nickname);
    new_user.setUsername(username);
    new_user.setSocket(socket);
	

    // Then we add the new user which connected to the server to the USER_LIST of the server
    this->setUserList(new_user);
                                                    
    return ;
}

std::string Server::createServerResponseForConnection(int socket, Server::userConnectionRegistration *userInfo) {

	bool	userWithSameNicknameExists = false;
	std::vector<User> userVector = this->getUserList();

	for (std::vector<User>::iterator it = userVector.begin(); it != userVector.end(); it++) {
		if ((*it).getNickname() == userInfo->nickName)
			userWithSameNicknameExists = true;
	}

	if (userInfo->password != this->_password) {
		std::cerr << "Error: client sent a wrong password to access the server" << std::endl;
		return "";
	}

	if (userWithSameNicknameExists == true) {
		std::cerr << "Error: This nickname already exists" << std::endl;
		return "";
	}

	createNewUserAtConnection(userInfo->nickName, userInfo->userName, socket);

	std::string server_response = ":localhost 001 " + userInfo->nickName + " :Welcome to the Internet Relay Network " + userInfo->nickName + "!" + userInfo->userName + "@localhost\r\n";

	return (server_response);
}

/********************************/
/*  CONNECTED SOCKET FUNCTIONS  */
/********************************/

int Server::retrieveDataFromConnectedSocket(int socketID, struct pollfd *fds, bool closeConnection) {

	char		buffer[512];
	int			recvReturn;
	User		*user;

	Server::clientData *clientData = getClientDataArray();

	clientData = &clientData[socketID];

	closeConnection = false;
	memset(buffer, 0, sizeof(buffer));
	while (1) {
		recvReturn = recv(fds[socketID].fd, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (recvReturn < 0) {
			if (errno != EWOULDBLOCK) {
				std::cerr << "Error: recv() failed" << std::endl;
				closeConnection = true;
			}
			return (closeConnection);
		}
		if (recvReturn == 0) {
			std::cerr << "Connection closed for socket " << socketID << std::endl;
			closeConnection = true;
			return (closeConnection);
		}
		clientData->dataString += buffer;
		if (clientData->dataString.find("\n") != std::string::npos)
			break ;
	}

	// Display for testing purpose
	std::cout << "** ======= SOCKET " << socketID <<  " ======== **" << std::endl;
	std::cout << "Buffer " << " : " << clientData->dataString << std::endl;

	// We handle the command here
	cmd command;
	user = this->getUserBySocket(fds[socketID].fd);
	command.whichCmd(clientData->dataString.c_str(), this, user);

	// We clear the dataString for the next data that will be in that socket and we get back to the poll() loop
	clientData->dataString.clear();
	return (closeConnection);
}
