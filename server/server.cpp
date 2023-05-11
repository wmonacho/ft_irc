#include "server.hpp"

// *** CONSTRUCTORS AND DESTRUCTOR ***

Server::Server() {

    _servLen = sizeof(_servAddr);
    _clientLen = sizeof(_clientAddr);
    this->_password = "";
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
    // this->_servAddr.sin_addr.s_addr = INADDR_ANY;
    this->_servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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

bool	Server::userInChannel(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	if (it_channel == this->getMap().end())
		return (false);
	return (it_channel->second.userInChannel(user));
}


//il faudra checker si le channel existe avant d'utiliser cette focntion
std::map<const User*, ChannelAspects>	Server::getChannelUserList(std::string channel_name)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);
	//throw une exception si possible en checkant s'il existe
	return (it_channel->second.getUserList());
}

//il faudra checker si le user existe avant d'utiliser cette focntion
const User* Server::getChannelUser(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	if (it_channel == this->getMap().end())
		return (nullptr);
	//throw une exception si possible a la place de return cet merde
	return (it_channel->second.getUser(user));
}

bool	Server::channelAlreadyExist(std::string channel_name)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	if (it_channel == this->getMap().end())
		return false;
	return true;
}

bool	Server::getUserAdmin(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	if (it_channel->second.getUserAdmin(user))
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
	//throw une exception si possible a la place de return cet merde
	User user;
	return (user);
}

std::string	Server::getChannelUserUsername(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	return (it_channel->second.getUserUsername(user));
}

std::string	Server::getChannelUserNickname(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	return (it_channel->second.getUserNickname(user));
}

std::string	Server::getChannelUserPassword(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	return (it_channel->second.getUserPassword(user));
}

std::string	Server::getChannelUserRealname(std::string channel_name, User *user)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	return it_channel->second.getChannelUserRealname(user);
}

std::string	Server::getChannelTopic(std::string	channel_name)
{
	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

	return (it_channel->second.getTopic());
}

//void	Server::kickUserFromChannel(std::string channel_name, User user)
//{
//	std::map<std::string, Channel>::iterator it_channel = this->getMap().find(channel_name);

//	it_channel->second.kickUserFromChannel(user);
//}
	//for (std::vector<User&>iterator it = this->getChannelUserList(channel_name, user).begin(); it != this->getChannelUserList(channel_name, user).end(); it++)
	//{
	//	if (it == user)
	//	{
	//		*it.erase();
	//		return ;
	//	}
	//}
	//std::cerr << "Error: user not in channel" << std::endl;
