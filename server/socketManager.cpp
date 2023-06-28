#include "server.hpp"
#include <cerrno>
#include <string.h>
#include <fcntl.h>

void    Server::startServer() {

	int				connectionStatus, pollReturn, socketID;
	int				nfds = 1;
	int				currentSize = 0;
	bool			closeConnection = false;
	bool			endOfServer = false;
	struct pollfd	fds[MAX_SOCKETS];
	
	// Initialize the array of pollfd structures
	memset(fds, 0, sizeof(fds));

	// Set the first struct of the array to the listening socket
	fds[0].fd = this->_socketfd;
	fds[0].events = POLLIN;

	Server::clientData *clientData = getClientDataArray();

	for (int i = 0; i < MAX_SOCKETS; i++) {
		clientData[i].clientIsConnected = false;
	}

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
				closeConnection = retrieveDataFromConnectedSocket(socketID, fds, closeConnection, &clientData[socketID]);
				if (closeConnection == true) {
					std::cerr << "/!!\\ Closing connection for socket " << socketID << " /!!\\" << std::endl;
					if (fds[socketID].fd != -1) {
						close(fds[socketID].fd);
						fds[socketID].fd = -1;
					}
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
	// int errorStatus = -1;

	do {
		newSocket = accept(this->_socketfd, NULL, NULL);
		if (newSocket < 0) { 
			std::cerr << "REMOVED EWOULDBLOCK --> ACCEPT returns < 0\n";
			break ;
		}
		// This is a new socket so we add it to our socket pool
		fds[nfds].fd = newSocket;
		fds[nfds].events = POLLIN;
		std::cout << "--A new socket is now connected to the server--" << std::endl;
		nfds++;
	} while (newSocket != -1);
	return (nfds);
}

int Server::verifyClientAndServerResponse(struct pollfd fds, Server::clientData *clientData) {

	// We retrieve the informations sent by the client to create the new user which connected to our server
	// and we send back the RPL_WELCOME

	Server::userConnectionRegistration *userInfo = &clientData->userConnectionRegistration;
	std::string server_response_for_connection;

	// We retrieve the connection informations from the socket and set flags if we find them
	if (getClientInformationsOnConnection(fds, userInfo, clientData->dataString) == false)
		return (0);
	// We create the string for the RPL_WELCOME and make a few checks (correct password and if the nickanme is not currently used)
	server_response_for_connection = createServerResponseForConnection(fds.fd, userInfo);
	if (server_response_for_connection.empty())
		return (0);

	// Finally we send back the server response to confirm the connection of the user
	send(fds.fd, server_response_for_connection.c_str(), server_response_for_connection.size(), 0);
	return (1);
}

bool	Server::findPassInBuffer(const char *buffer, Server::userConnectionRegistration *userInfo) {

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

bool	Server::findNickInBuffer(const char *buffer, Server::userConnectionRegistration *userInfo) {

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

bool	Server::findUserInBuffer(const char *buffer, Server::userConnectionRegistration *userInfo) {

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

bool Server::getClientInformationsOnConnection(struct pollfd fds, Server::userConnectionRegistration *userInfo, std::string data) {

	(void)fds;
	if (userInfo->passCheck == false)
		userInfo->passCheck = findPassInBuffer(data.c_str(), userInfo);
	if (userInfo->passCheck == true) {
		if (userInfo->nickCheck == false)
			userInfo->nickCheck = findNickInBuffer(data.c_str(), userInfo);
		if (userInfo->userCheck == false)
			userInfo->userCheck = findUserInBuffer(data.c_str(), userInfo);
	}
	if (userInfo->passCheck == true && userInfo->nickCheck == true && userInfo->userCheck == true) {
		
		std::cout << "== ALL CONNECTION INFORMATIONS RETRIEVED ==" << std::endl;
		return true;
	}

	return false;
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

	if (userInfo->nickName.empty() || userInfo->nickName == "")
	{
		std::string response = std::string(":localhost ") + "431 " + userInfo->nickName + " " + ":No nickname given" + "\r\n";
		send(socket, response.c_str(), response.size(), 0);
		userInfo->nickCheck = false;
		return "";
	}

	if (userInfo->nickName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", 0) != std::string::npos) {
		std::string response = std::string(":localhost ") + "432 " + userInfo->nickName + " " + userInfo->nickName + " :Erroneous nickname" + "\r\n";
		send(socket, response.c_str(), response.size(), 0);
		userInfo->nickCheck = false;
		return "";
	}

	if (userInfo->password != this->_password) {
		std::string response = std::string(":localhost ") + "464 " + userInfo->nickName + " " + ":Password incorrect" + "\r\n";
		send(socket, response.c_str(), response.size(), 0);
		userInfo->passCheck = false;
		return "";
	}

	if (userWithSameNicknameExists == true) {
 		std::string response = std::string(":localhost ") + "433 " + userInfo->nickName + " " + userInfo->nickName + " :Nickname is already in use" + "\r\n";
		send(socket, response.c_str(), response.size(), 0);
		userInfo->nickCheck = false;
		return "";
	}

	createNewUserAtConnection(userInfo->nickName, userInfo->userName, socket);
	std::string server_response = ":localhost 001 " + userInfo->nickName + " :Welcome to the Internet Relay Network " + userInfo->nickName + "!" + userInfo->userName + "@localhost\r\n";
	return (server_response);
}

/********************************/
/*  CONNECTED SOCKET FUNCTIONS  */
/********************************/

int Server::retrieveDataFromConnectedSocket(int socketID, struct pollfd *fds, bool closeConnection, Server::clientData *clientData) {

	char		buffer[512];
	int			recvReturn;
	int			connectionDone;
	int			clientStatus;
	User		*user;
	cmd			command;

	closeConnection = false;
	while (1) {
		memset(buffer, 0, sizeof(buffer));
		recvReturn = recv(fds[socketID].fd, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (recvReturn < 0) {
			std::cerr << "REMOVED EWOULDBLOCK --> RECV returns < 0\n";
			return (closeConnection);
		}
		if (recvReturn == 0) {
			std::cerr << "****** Connection closed for socket : " << socketID << std::endl;
			command.parseQuit(this, "QUIT :Leaving", this->getUserBySocket(fds[socketID].fd));
			closeConnection = true;
			return (closeConnection);
		}
		clientData->dataString += buffer;

		// If we get a correct request, we can use it, otherwise we try to receive what is left
		if (!clientData->dataString.empty() && (clientData->dataString.find("\n") != std::string::npos)) {

			if (clientData->clientIsConnected == false) {
				connectionDone = verifyClientAndServerResponse(fds[socketID], clientData);
				if (connectionDone == 1)
					clientData->clientIsConnected = true;
				else {
					clientData->clientIsConnected = false;
					clientData->dataString.clear();
				}
			}

			if (clientData->clientIsConnected == true) {

				// Display for testing purpose
				std::cout << "------======= HANDLING SOCKET " << socketID <<  " ========------" << std::endl;
				std::cout << "========Buffer========\n" << clientData->dataString;
				std::cout << "======================" << std::endl;

				// We handle the command here
				user = this->getUserBySocket(fds[socketID].fd);
				clientStatus = command.whichCmd(clientData->dataString.c_str(), this, user);
				if (clientStatus == 2) {
					clientData->clientIsConnected = false;
					closeConnection = true;
				}

				// We clear the dataString for the next data that will be in that socket and we get back to the poll() loop
				clientData->dataString.clear();
				return (closeConnection);
			}
		}
	}
}
