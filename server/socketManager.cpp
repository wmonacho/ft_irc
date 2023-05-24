#include "server.hpp"

void    Server::startServer() {

    // We create a socket for client/server communication
    int             connectionStatus, pollReturn, socketID;
    int             nfds = 1;
    int             currentSize = 0;
    bool            closeConnection = false;
    bool            endOfServer = false;
    struct pollfd   fds[42];
    
    // Initialize the array of pollfd structures
    memset(fds, 0, sizeof(fds));

    // Set the first struct of the array to the listening socket
    fds[0].fd = this->_socketfd;
	fds[0].events = POLLIN;

    // This is the main loop which implements poll() : we detect if the socket is connecting or connected and act in consequence
    do {
        pollReturn = poll(fds, nfds, (1 * 60 * 1000)); // 1 min timeout
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
            // revents should be POLLIN 
            // if (fds[socketID].revents != POLLIN) {
            //     std::cerr << "Error: revents: " << fds[socketID].revents << std::endl;
            //     endOfServer = true;
            //     break ;
            // }
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
        if (fds[i].fd >= 0)
            close(fds[i].fd);
    }

    return ;
}

/*******************************/
/* CONNECTING SOCKET FUNCTIONS */
/*******************************/

int Server::acceptNewConnection(struct pollfd *fds, int nfds) {

    int newSocket;
    int errorStatus = -1;

    do {
        newSocket = accept(this->_socketfd, NULL, NULL);
        if (newSocket < 0) {
            if (errno != EWOULDBLOCK) {
                std::cerr << "Error: accept() failed" << std::endl;
                nfds = errorStatus;
            }
            std::cout << "Socket is already connected" << std::endl;
            break ;
        }
        fds[nfds].fd = newSocket;
        fds[nfds].events = POLLIN;
        if (verifyClientAndServerResponse(fds[nfds]) == 1)
            return (-1);
        nfds++;
    } while (newSocket != -1);

    return (nfds);
}

int Server::verifyClientAndServerResponse(struct pollfd fds) {

    // We retrieve the informations sent by the client to create the new user which connected to our server
    // and we send back the RPL_WELCOME

    std::string server_response_for_connection;

    // We retrieve the data from the socket in buffer
    std::string buffer = getClientInformationsOnConnection(fds);
    if (buffer.empty()) {
        std::cerr << "Error: couldn't retrieve client information on connection" << std::endl;
        return (1);
    }

    // This function parse the buffer to find the username and nickname of the user who connected to the server
    // and it creates a new user in the server's users_list
    server_response_for_connection = createServerResponseForConnection(buffer, fds.fd);

    // Finally we send back the server response to confirm the connection of the user
    send(fds.fd, server_response_for_connection.c_str(), server_response_for_connection.size(), 0);
    return (0);
}

std::string Server::getClientInformationsOnConnection(struct pollfd fds) {
        
    int     bytesRead;
    char    buffer[512];
	
    memset(buffer, 0, 511);

    //We read the socket to get the client information that we will use to create the new user (whose connected)
	bytesRead = recv(fds.fd, buffer, 511, 0);
    if (bytesRead <= 0) {
        std::cerr << "Error: recv() failed for connection registration: empty socket" << std::endl;
    }
    return (std::string(buffer, bytesRead));
}

void    Server::createNewUserAtConnection(std::string nickname, std::string username, int socket) {

    // We create a new user and set his nickname and realname thanks to the message the client sent
    User new_user;

    new_user.setNickname(nickname);
    new_user.setRealname(username);
    new_user.setSocket(socket);

    // Then we add the new user which connected to the server to the USER_LIST of the server
    this->setUserList(new_user);

    return ;
}

std::string Server::createServerResponseForConnection(std::string buffer, int socket) {

    size_t nickPos = buffer.find("NICK");
    size_t userPos = buffer.find("USER");
    if (nickPos == std::string::npos || userPos == std::string::npos) {
        std::cout << "Error: couldn't retrieve connection informations: NICK or USER missing in registration information" << std::endl;
        exit(1); //THROW EXCEPTION HERE
    }
    // We get past NICK and USER to only get the nickname and username
    nickPos += 5;
    userPos += 5;
    std::string nickName = buffer.substr(nickPos);
    size_t limiter = nickName.find("\r\n");
    nickName = nickName.substr(0, limiter);
    // std::cout << nickName << std::endl;
    std::string userName = buffer.substr(userPos, nickName.size());
    // std::cout << userName << std::endl;

    createNewUserAtConnection(nickName, userName, socket);

    std::string server_response = ":localhost 001 " + userName + " :Welcome to the Internet Relay Network " + nickName + "!" + userName + "@localhost\r\n";

    return (server_response);
}

/********************************/
/*  CONNECTED SOCKET FUNCTIONS  */
/********************************/

int Server::retrieveDataFromConnectedSocket(int socketID, struct pollfd *fds, bool closeConnection) {

    char    buffer[512];
    int     recvReturn;

    closeConnection = false;
    memset(buffer, 0, sizeof(buffer));
    recvReturn = recv(fds[socketID].fd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (recvReturn < 0) {
        if (errno != EWOULDBLOCK) {
            std::cerr << "Error: recv() failed" << std::endl;
            closeConnection = true;
        }
        return (closeConnection);
    }
    if (recvReturn == 0) {
        std::cerr << "Connection closed" << std::endl;
        closeConnection = true;
        return (closeConnection);
    }

    // Display for testing purpose
    std::cout << "** =============== **" << std::endl;
    // Affichage sur le serveur
    std::cout << "Buffer from socket " << socketID << " : " << buffer << std::endl;

    // Loop to identify which user sent a message to send it to the whichCmd()
    for (std::vector<User>::iterator user = _user_list.begin(); user != _user_list.end(); user++) {
        if (user->getSocket() == fds[socketID].fd)
            break ;
    }
    // HANDLE CLIENT MESSAGE HERE
    // whichCmd(buffer, this, *it);
    
    // We send the message back to the client (TESTING PURPOSE)
    send(fds[socketID].fd, buffer, recvReturn, 0);

    return (closeConnection);
}