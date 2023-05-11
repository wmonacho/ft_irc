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

class Channel;
class Server {

    private :

        int _port;
        int _socketfd;
        int _newSocket;
        ssize_t _valRread;
        socklen_t   _servLen;
        socklen_t   _clientLen;
        std::string _password;
        std::vector<User>   _user_list;
        struct sockaddr_in  _servAddr;
        struct sockaddr_in  _clientAddr;
        std::map<std::string, Channel>  _channels;

    public :

        Server();
        Server(int port, std::string password);
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        int                             getSocketfd(void);
        int                             getNewSocket(void);
        ssize_t                         getValRead(void);
        int                             getPort(void);
        socklen_t                       getServLen(void);
        socklen_t*                      getClientLen(void);
        sockaddr_in*                    getServAddr(void);
        sockaddr_in*                    getClientAddr(void);
        std::string                     getPassword(void);
        std::vector<User>               getUserList(void);
        std::map<std::string, Channel>  getMap(void);

        void    setSocketfd(int fd);
        void    setNewSocket(int fd);
        void    setValRead(ssize_t value);
        void    setPort(int port);
        void    setServAddr(int port);
        void    setUserList(User new_user);
        void    setPassword(std::string new_password);
        void    setNewChannelInMap(const Channel& channel);
        void	createRandomUsername( User user );

        bool    alreadyRegistred( void );
        bool    nickAlreadyExist( std::string new_nick );
        bool    usernameAlreadyExist( std::string new_username );
		bool	userIsInChannel(std::string channel_name, User user);
		bool 	channelAlreadyExist(std::string channel_name);
		bool	getUserAdmin(std::string channel_name, User user);

		User	getChannelUser(std::string channel_name, User user);

		std::string	getChannelUserUsername(std::string channel_name, User user);
		std::string	getChannelUserRealname(std::string channel_name, User user);
		std::string	getChannelUserPassword(std::string channel_name, User user);
		std::string	getChannelUserNickname(std::string channel_name, User user);
		std::string	getChannelTopic(std::string	channel_name);

		std::vector<User> getChannelUserList(std::string channel_name);

        void    startServer(Server* obj);

};

#endif
