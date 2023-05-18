#ifndef SERVER_HPP
# define SERVER_HPP

#include "../channel/Channel.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <poll.h>
#include <map>

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

        // Setters functions
        void   	                            setPort(int port);
        void   	                            setSocketfd(int fd);
        void   	                            setNewSocket(int fd);
        void   	                            setServAddr(int port);
        void   	                            setValRead(ssize_t value);
        void   	                            setUserList(User new_user);
        void   	                            setPassword(std::string new_password);
        void   	                            setNewChannelInMap(const Channel& channel);
		void					            setUserUsername(User user, std::string new_username);
		void					            setUserNickname(User user, std::string new_nickname);
		void					            setUserPassword(User user, std::string new_password);
		void					            setUserRealname(User user, std::string new_realname);
        
        // Getters functions
        int                                 getPort(void);
        int                                 getSocketfd(void);
        int                                 getNewSocket(void);
		std::string		                    getChannelTopic(std::string	channel_name);
        std::string 	                    getPassword(void);
		const std::string		            getChannelUserUsername(std::string channel_name, User *user);
		const std::string		            getChannelUserRealname(std::string channel_name, User *user);
		const std::string		            getChannelUserPassword(std::string channel_name, User *user);
		const std::string		            getChannelUserNickname(std::string channel_name, User *user);      
		bool	                            getChannelUserAdmin(std::string channel_name, User *user);
        ssize_t		                        getValRead(void);
        socklen_t                           getServLen(void);
        socklen_t*                          getClientLen(void);
		sockaddr_in*	                    getServAddr(void);
        sockaddr_in*	                    getClientAddr(void);
		User   		                        getUser(std::string user_nickname);
		const User*	                        getChannelUser(std::string channel_name, const User *user);
		std::vector<User>                       getUserList(void);
        std::map<std::string, Channel>          getMap(void);
        std::map<const User*, ChannelAspects>   getChannelUserList(std::string channel_name);

        // Socket connection and user registration for "socketManager.cpp"
        int                                 verifyClientAndServerResponse(struct pollfd fds);
        int                                 acceptNewConnection(struct pollfd *fds, int nfds);
        int                                 retrieveDataFromConnectedSocket(int socketID, struct pollfd *fds, bool closeConnection);
        std::string                         createServerResponseForConnection(std::string buffer);
        std::string                         getClientInformationsOnConnection(struct pollfd fds);
        void                                createNewUserAtConnection(std::string nickname, std::string username);
 

        // Utility functions
        void                                startServer();
		void	                            kickUserFromChannel(std::string channel_name, User user);
        void	                            createRandomUsername( User user );
		bool   	                            passwordAlreadyRegistred( void );
        bool   	                            nickAlreadyExist( std::string new_nick );
		bool 	                            channelAlreadyExist(std::string channel_name);
        bool   	                            usernameAlreadyExist( std::string new_username );
		bool	                            userInChannel(std::string channel_name, const User *user);
};

#endif
