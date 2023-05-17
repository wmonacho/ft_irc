#include "server.hpp"
#include <iomanip>

// *** CONSTRUCTORS AND DESTRUCTOR ***

Server::Server() {}

Server::Server(const Server& obj) {

    this->_port = obj._port;
    this->_socketfd = obj._socketfd;
    this->_newSocket = obj._newSocket;
    this->_valRread = obj._valRread;
    this->_servLen = obj._servLen;
    this->_clientLen = obj._clientLen;
    this->_servAddr = obj._servAddr;
    this->_clientAddr = obj._clientAddr;
    *this = obj;
    return ;
}

Server& Server::operator=(const Server& obj) {

    this->_port = obj._port;
    this->_socketfd = obj._socketfd;
    this->_newSocket = obj._newSocket;
    this->_valRread = obj._valRread;
    this->_servLen = obj._servLen;
    this->_clientLen = obj._clientLen;
    this->_servAddr = obj._servAddr;
    this->_clientAddr = obj._clientAddr;
    // *this = obj; #BOUCLE INFINI OPE=
    return *this;
}

Server::~Server() {}

Server::Server(int port, std::string password) {

    // We create and initiate a new server, a listening socket is created and he's listening for connection
    _servLen = sizeof(_servAddr);
    _clientLen = sizeof(_clientAddr);
    _port = port;
    _password = password;
    int on = 1;

    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketfd < 0) {
        std::cout << "Error: socket creation failed" << std::endl;
        exit(1);
    }

    // Allow socket descriptor to be reuseable 
    setsockopt(_socketfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    // Set the socket to be non-blocking (the sockets created after will inherit)
    ioctl(_socketfd, FIONBIO, (char *)&on);
    setServAddr(_port);

    if (bind(_socketfd, (struct sockaddr*)&_servAddr, sizeof(_servAddr)) < 0) {
        std::cout << "Error : socket binding failed" << std::endl;
		exit(1);
    }

    // ATTENTIOn au deuxieme arg (backlog queue)
    if (listen(_socketfd, 6) < 0) {
		std::cout << "Error : listen failed" << std::endl;
		exit(1);
    }

    return ;
}

void    Server::startServer() {

    // We create a socket for client/server communication
    int     newSocket, recvReturn, pollReturn;
    int     nfds = 1;
    int     currentSize = 0;
    int     i;
    bool    closeConnection = false;
    bool    endOfServer = false;
    char    buffer[1024];
    struct pollfd fds[42];

    // std::cout << "Server got a connection from " << inet_ntoa(this->_clientAddr.sin_addr) << " on port " << this->_clientAddr.sin_port << std::endl;
    
    // Initialize the array of pollfd structures
    memset(fds, 0, sizeof(fds));
    // Set the first struct to the listenning socket
    fds[0].fd = this->_socketfd;
	fds[0].events = POLLIN;

    do {
        // Thanks to poll we can check if something happened to a socket
        std::cout << "NFDS : " << nfds << std::endl;
        pollReturn = poll(fds, nfds, -1);
        if (pollReturn < 0) {
            std::cerr << "Error: poll() failed" << std::endl;
            break ;
        }
        if (pollReturn == 0) {
            std::cerr << "Error: poll() timed out" << std::endl;
            break ;
        }
        // pollReturn indicates that something happened on one of our socket
        // We set the currentSize to the number of sockets in our pollfd array
        currentSize = nfds;
        for (i = 0; i < currentSize; i++) {
            std::cout << "loop [i] = " << i << std::endl;
            // if (fds[i].revents == 0) {
            //     ;
            // }
            // revents should be POLLIN 
            // if (fds[i].revents != POLLIN) {
            //     std::cerr << "Error: revents: " << fds[i].revents << std::endl;
            //     endOfServer = true;
            //     break ;
            // }
            // If it is a connecting socket we accept the connection and add it to the socket pool 
            if (fds[i].fd == this->_socketfd) {
                std::cerr << "== New socket connection ==" << std::endl;
                do {
                    newSocket = accept(this->_socketfd, NULL, NULL);
                    std::cout << "newSocket : " << newSocket << std::endl;
                    if (newSocket < 0) {
                        if (errno != EWOULDBLOCK) {
                            std::cerr << "Error: accept() failed" << std::endl;
                            endOfServer = true;
                        }
                        std::cout << "Socket is already connected" << std::endl;
                        break ;
                    }
                    fds[nfds].fd = newSocket;
                    fds[nfds].events = POLLIN;
                    std::cerr << "Parse and send RPL_WELCOME" << nfds << std::endl;
                    parseAndConnect(fds[nfds]);
                    nfds++;
                } while (newSocket != -1);
            }
            // It is already a connected socket so we try to receive the data
            else {
                closeConnection = false;
                do {
                    std::cout << "Connected socket: fds[i] --> " << i << std::endl;
                    memset(buffer, 0, sizeof(buffer));
                    recvReturn = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (recvReturn < 0) {
                        if (errno != EWOULDBLOCK) {
                            std::cerr << "Error: recv() failed" << std::endl;
                            closeConnection = true;
                        }
                        std::cout << "Breaking connection loop" << std::endl;
                        break ;
                    }
                    if (recvReturn == 0) {
                        std::cerr << "Connection closed" << std::endl;
                        closeConnection = true;
                        break ;
                    }
                    std::cout << "===============" << std::endl;
                    std::cout << buffer << std::endl;
                    send(fds[i].fd, buffer, recvReturn, 0);
                } while (true);
                if (closeConnection) {
                    std::cout << "Closing connection for socket " << i << std::endl;
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
            }
        }
    } while (endOfServer == false);
    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd >= 0)
            close(fds[i].fd);
    }
    return ;
}

int Server::parseAndConnect(struct pollfd fds) {

    // We retrieve the information sent by the client to create the new user that connected to our server
    // and we send back the RPL_WELCOME

    char    buff[1024];

    // poll loop to get connection informations from the client and send RPL_WELCOME
	memset(buff, 0, 1023);
    while (1) {
        int num_events = poll(&fds, 1, -1);
		if (num_events == -1) {
			std::cout << "Poll error" << std::endl;
			exit(1);
		}
		else if (num_events == 0) {
			std::cout << "Client closed the connection" << std::endl;
		}
		else {
			if (fds.revents & POLLIN) {
				recv(fds.fd, buff, 1023, 0);
                if (buff != NULL)
                    break ;
            }
        }
	}

    // Parse buff to get informations about the newly connected user and send RPL_WELCOME
    std::string buffer = buff;

    std::cout << "= Parse and connect =" << std::endl;

    size_t nickPos = buffer.find("NICK");
    size_t userPos = buffer.find("USER");
    if (nickPos == std::string::npos || userPos == std::string::npos) {
        std::cout << "Error while retrieving connection informations" << std::endl;
        exit(1);
    }
    nickPos += 5;
    userPos += 5;
    std::string nickName = buffer.substr(nickPos);
    size_t limiter = nickName.find("\r\n");
    nickName = nickName.substr(0, limiter);
    std::cout << nickName << std::endl;
    std::string userName = buffer.substr(userPos, nickName.size());
    std::cout << userName << std::endl;

    // We create a new user and set his nickname and realname thanks to the message the client sent
    User new_user;
    new_user.setNickname(nickName);
    new_user.setRealname(userName);
    // we add the new user that connected to the server to the USER_LIST of the server
    this->setUserList(new_user);

    std::string server_response = ":localhost 001 " + userName + " :Welcome to the Internet Relay Network " + nickName + "!" + userName + "@localhost\r\n";
    // std::cout << server_response << std::endl;
    send(fds.fd, server_response.c_str(), server_response.size(), 0);
    std::cout << "= end of parse / connection done =" << std::endl;
    return (0);
}


// *** GETTERS ***

int Server::getSocketfd(void) {

    return this->_socketfd;
}

int Server::getNewSocket(void) {

    return this->_newSocket;
}

ssize_t Server::getValRead(void) {

    return this->_valRread;
}

int Server::getPort(void) {

    return this->_port;
}

socklen_t Server::getServLen(void) {

    return this->_servLen;
}

socklen_t* Server::getClientLen(void) {

    return &this->_clientLen;
}

sockaddr_in* Server::getServAddr(void) {

    return &this->_servAddr;
}

sockaddr_in* Server::getClientAddr(void) {

    return &this->_clientAddr;
}
std::string Server::getPassword(void) 
{
    return (this->_password);
}

std::vector<User>  Server::getUserList(void)
{
    return (this->_user_list);
}
    
std::map<std::string, Channel>  Server::getMap(void) {

    return this->_channels;
}

const std::string	Server::getChannelUserUsername(std::string channel_name, User *user)
{
	return (this->getMap().find(channel_name)->second.getUserUsername(user));
}

const std::string	Server::getChannelUserNickname(std::string channel_name, User *user)
{
	return (this->getMap().find(channel_name)->second.getUserNickname(user));
}

const std::string	Server::getChannelUserPassword(std::string channel_name, User *user)
{
	return (this->getMap().find(channel_name)->second.getUserPassword(user));
}

const std::string	Server::getChannelUserRealname(std::string channel_name, User *user)
{
	return this->getMap().find(channel_name)->second.getUserRealname(user);
}

std::string	Server::getChannelTopic(std::string	channel_name)
{
	return (this->getMap().find(channel_name)->second.getTopic());
}

bool	Server::getChannelUserAdmin(std::string channel_name, User *user)
{
	if (this->getMap().find(channel_name)->second.getUserAdmin(user))
		return true;
	return false;
}

User	Server::getUser(std::string user_nickname)
{
	for (std::vector<User>::iterator it = this->_user_list.begin(); it != this->_user_list.end(); it++)
	{
		if (it->getNickname() == user_nickname)
			return (*it);
	}
	//throw une exception si possible a la place de return cette merde
	User user;
	return (user);
}

// *** SETTERS ***

void    Server::setSocketfd(int fd) {

    this->_socketfd = fd;
}

void    Server::setNewSocket(int fd) {

    this->_newSocket = fd;
}

void    Server::setValRead(ssize_t value) {

    this->_valRread = value;
}

void    Server::setPort(int port) {

    this->_port = port;
}

void    Server::setServAddr(int port) {

    // Server byte order
    this->_servAddr.sin_family = AF_INET;
    // Fill with current host's IP address
    this->_servAddr.sin_addr.s_addr = INADDR_ANY;
    // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order
    this->_servAddr.sin_port = htons(port);
}

void    Server::setPassword(std::string new_password)
{
    this->_password = new_password;
}

// Add a new user to the user_list (std::vector type) of the server
void    Server::setUserList(User new_user)
{
    this->_user_list.push_back(new_user);
}

void    Server::setNewChannelInMap(const Channel& channel) {

    this->_channels[channel.getName()] = channel;
}

void	Server::setUserUsername(User user, std::string new_username)
{
	user.setUsername(new_username);
}

void	Server::setUserNickname(User user, std::string new_nickname)
{
	user.setNickname(new_nickname);
}

void	Server::setUserPassword(User user, std::string new_password)
{
	user.setPassword(new_password);
}

void	Server::setUserRealname(User user, std::string new_realname)
{
	user.setRealname(new_realname);
}

/* SERVER FUNCTIONS */

bool    Server::passwordAlreadyRegistred( void )
{
    if (this->_password == "")
        return (0);
    return (1);
}

bool    Server::nickAlreadyExist( std::string new_nick )
{
    for(unsigned int i = 0; i < this->_user_list.size(); i++)
    {
        if (this->_user_list[i].getNickname() == new_nick)
            return (0);
    }
    return (1);
}

bool    Server::usernameAlreadyExist( std::string new_username )
{
    for(unsigned int i = 0; i < this->_user_list.size(); i++)
    {
        if (this->_user_list[i].getNickname() == new_username)
            return (0);
    }
    return (1);
}


//Cree un Username unique pour qu'aucun User ne possede un username par defaut identique
void	Server::createRandomUsername( User user )
{
    int i = 1;
    int j = 0;

    std::stringstream   str;
    str << i;
    std::string id = str.str();
    std::string new_username = "Guest" + id;
    while (this->usernameAlreadyExist(new_username) || j < 10000)
    {
    	i = rand() % 999;
        str << i;
        id = str.str();
         j++;
    }
    user.setUsername("Guest" + id);
}

bool	Server::userInChannel(std::string channel_name, const User *user)
{
	if (this->getMap().find(channel_name) == this->getMap().end())
		return (false);
	return (this->getMap().find(channel_name)->second.userInChannel(user));
}


//il faudra checker si le channel existe avant d'utiliser cette focntion
std::map<const User*, ChannelAspects>	Server::getChannelUserList(std::string channel_name)
{
	//throw une exception si possible en checkant s'il existe
	return (this->getMap().find(channel_name)->second.getUserList());
}

//il faudra checker si le user existe avant d'utiliser cette focntion
const User* Server::getChannelUser(std::string channel_name, const User *user)
{
	//throw une exception si possible a la place de return cet merde
	return (this->getMap().find(channel_name)->second.getUser(user));
}

bool	Server::channelAlreadyExist(std::string channel_name)
{
	if (this->getMap().find(channel_name) == this->getMap().end())
		return false;
	return true;
}

// bool	Server::topicAlreadyExist(std::string channel_name)
// {

// 	if (this->getChannelTopic( channel_name).size() < 1)
// 		return false;
// 	return true;
// }
//void	Server::kickUserFromChannel(std::string channel_name, User user)
//{
//	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

//	it_channel->second.kickUserFromChannel(user);
//}
