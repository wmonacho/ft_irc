#include "server.hpp"
#include <iomanip>

// *** CONSTRUCTORS AND DESTRUCTOR ***

Server::Server() {}

Server::Server(int port, std::string password) {

    _servLen = sizeof(_servAddr);
    _clientLen = sizeof(_clientAddr);
    _port = port;
    _password = password;

    _socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketfd < 0) {
        std::cout << "Error: socket creation failed" << std::endl;
        exit(1);
    }

    setServAddr(_port);

    if (bind(_socketfd, (struct sockaddr*)&_servAddr, sizeof(_servAddr)) < 0) {
        std::cout << "Error : socket binding failed" << std::endl;
		exit(1);
    }

    if (listen(_socketfd, 3) < 0) {
		std::cout << "Error : listen failed" << std::endl;
		exit(1);
    }

    startServer(this);
    return ;
}

void    Server::startServer(Server* server) {

    int     newSocket;
    char    buffer[1024];
    struct pollfd fds;

    // accept() will create a new socket and give a fd related to it and write the information of the remote_host in the struct we give
    newSocket = accept(server->_socketfd, (struct sockaddr*)&server->_clientAddr, &server->_clientLen);
    if (newSocket < 0) {
		std::cout << "Error : new socket creation failed" << std::endl;
		exit(1);
    }

    std::cout << "Server got a connection from " << inet_ntoa(server->_clientAddr.sin_addr) << " on port " << server->_clientAddr.sin_port << std::endl;

    fds.fd = newSocket;
	fds.events = POLLIN;

    int bytesReceived;
    bytesReceived = recv(newSocket, buffer, 1023, 0);
    if (bytesReceived <= 0) {
        close(newSocket);
        close(server->_socketfd);
        std::cout << "Error" << std::endl;
        exit(1);
    }

    std::cout << "Buffer : " << std::endl;
    std::cout << buffer << "\n" << std::endl; 

	while (1) {
		int num_events = poll(&fds, 1, -1); // Wait indefinitely for events
		if (num_events == -1) {
			// Handle poll error
			std::cout << "Poll error" << std::endl;
			close(newSocket);
			close(server->_socketfd);
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
				if (recv(newSocket, buffer, 1023, 0) > 0) {
					std::cout << "Message from the client : " << buffer;
                }
			}
		}
	}

    return ;
}

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
    // this->_servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order
    this->_servAddr.sin_port = htons(port);
}

void    Server::setPassword(std::string new_password)
{
    this->_password = new_password;
}

void    Server::setUserList(User new_user)
{
    this->_user_list.push_back(new_user);
}

void    Server::setNewChannelInMap(const Channel& channel) {

    this->_channels[channel.getName()] = channel;
}


/* SERVER FUNCTIONS */

void    Server::joinChannel( std::string channel_name, User new_user )
{
    for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
        if (channel_name == it->second.getName()) {
    /*rejoindre le User dans le Channel deja existant*/
            it->second.setUserList(new_user);
            return ;
        }
            /*sinon creer un nouveau Channel y ajouter le User avec les droits admin et utiliser setNewChannelInMap ensuite*/
    Channel new_channel(channel_name);
    new_user.setAdmin(1);
    new_channel.setUserList(new_user);
    setNewChannelInMap(new_channel);
}

bool    Server::alreadyRegistred( void )
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
        new_username = "Guest" + id;
        j++;
    }
    user.setUsername("Guest" + id);
}