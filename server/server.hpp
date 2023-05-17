#ifndef SERVER_HPP
# define SERVER_HPP

#include "../channel/Channel.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <map>
#include <algorithm>

class Channel;
class User;
class ChannelApects;
class Server {

    private :

        int _port;
        int _socketfd;
        int _newSocket;
        ssize_t _valRread;
        socklen_t   _servLen;
        socklen_t   _clientLen;
        std::string _password;
        std::vector<User>	_user_list;
        struct sockaddr_in  _servAddr;
        struct sockaddr_in  _clientAddr;
        std::map<std::string, Channel>  _channels;

    public :

        Server();
        Server(int port, std::string password);
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        int		getPort(void);
        int		getSocketfd(void);
        int		getNewSocket(void);

        ssize_t		getValRead(void);
        socklen_t   getServLen(void);
        socklen_t*  getClientLen(void);

        std::string 	getPassword(void);
        
		sockaddr_in*	getServAddr(void);
        sockaddr_in*	getClientAddr(void);
		
        void   	setPort(int port);
        void   	setSocketfd(int fd);
        void   	setNewSocket(int fd);
        void   	setServAddr(int port);
        void   	setValRead(ssize_t value);
        void   	setUserList(User new_user);
        void   	setPassword(std::string new_password);
        void   	setNewChannelInMap(const Channel& channel);
		void	setUserUsername(User user, std::string new_username);
		void	setUserNickname(User user, std::string new_nickname);
		void	setUserPassword(User user, std::string new_password);
		void	setUserRealname(User user, std::string new_realname);
        
        
		bool		getChannelUserAdmin(std::string channel_name, User *user);
		User   		getUser(std::string user_nickname);
		const User*	getConstUser(std::string user_nickname);
		const User*	getChannelUser(std::string channel_name, const User *user);
		const User* getChannelUser(std::string channel_name, std::string user_name);
		std::string		        getChannelTopic(std::string	channel_name);
		const std::string		getChannelUserUsername(std::string channel_name, User *user);
		const std::string		getChannelUserRealname(std::string channel_name, User *user);
		const std::string		getChannelUserPassword(std::string channel_name, User *user);
		const std::string		getChannelUserNickname(std::string channel_name, User *user);
		std::vector<User>                       getUserList(void);
        std::map<std::string, Channel>          getMap(void);
        std::map<const User*, ChannelAspects>   getChannelUserList(std::string channel_name);
        
		void	kickUserFromChannel(std::string channel_name, User user);
        void	createRandomUsername( User user );
		void    startServer();
		bool   	passwordAlreadyRegistred( void );
        bool   	nickAlreadyExist( std::string new_nick );
		bool 	channelAlreadyExist(std::string channel_name);
        bool   	usernameAlreadyExist( std::string new_username );
		bool	userInChannel(std::string channel_name, const User *user);
        int     parseAndConnect(std::string buffer, int socket);
    
};

#endif
