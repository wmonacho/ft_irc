#ifndef SERVER_HPP
# define SERVER_HPP

#include "../channel/Channel.hpp"
#include "../cmd/cmd.hpp"
#include "../bot/bot.hpp"
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
#include <algorithm>

#define MAX_SOCKETS 42

class Channel;
class User;
class ChannelApects;
class bot;

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
        std::map<std::string, Channel*>  _channels;
	 bool _botConnected;

		struct userConnectionRegistration {
			std::string	password;
			std::string	nickName;
			std::string	userName;
		};
		struct userConnectionRegistration _userConnectionRegistration;
		

    public :

        Server();
        Server(int port, std::string password);
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        bool    check;
        // Setters functions
        void   	                                setPort(int port);
        void   	                                setSocketfd(int fd);
        void   	                                setNewSocket(int fd);
        void   	                                setServAddr(int port);
        void   	                                setValRead(ssize_t value);
        void   	                                setUserList(User new_user);
        void   	                                setPassword(std::string new_password);
        void				                 	setUserUsername(User user, std::string new_username);
        void				    	            setUserNickname(User user, std::string new_nickname);
        void		            	          	setUserPassword(User user, std::string new_password);
        void				                  	setUserRealname(User user, std::string new_realname);
		void									setUserConnectionResitrationStruct(std::string pass, std::string nick, std::string user);
        
        // Getters functions
        int                              		getPort(void);
        int                              		getSocketfd(void);
        int                              		getNewSocket(void);
        std::string	                    		getChannelTopic(std::string	channel_name);
        std::string 	                    	getPassword(void);
        const std::string                		getChannelUserUsername(std::string channel_name, User *user);
        const std::string                		getChannelUserRealname(std::string channel_name, User *user);
        const std::string                		getChannelUserPassword(std::string channel_name, User *user);
        const std::string                		getChannelUserNickname(std::string channel_name, User *user);
        bool	                           		getChannelUserAdmin(std::string channel_name, User *user);
        bool									getChannelInviteOnly(std::string channel_name);
        ssize_t                          		getValRead(void);
        socklen_t                        		getServLen(void);
        socklen_t*                       		getClientLen(void);
        sockaddr_in*	                        getServAddr(void);
        sockaddr_in*	                        getClientAddr(void);
        User* 		                            getUser(std::string user_nickname);
        User*	                                getUserWithName(std::string user_name);
		User*									getUserBySocket(int socket);
		Channel*                                getChannel(std::string channel_name);
        const User* 	                        getConstUser(std::string user_nickname);
        const User*                             getChannelUser(std::string channel_name, std::string user_name);
        const User*	                            getChannelUser(std::string channel_name, const User *user);
        std::vector<User>							getUserList(void);
        const std::map<std::string, Channel*>&			getMap(void); //read only
        const std::map<const User*, UserAspects>&   getChannelUserList(std::string channel_name);
		std::map<std::string, Channel*>::iterator	getItMap(void);
		userConnectionRegistration*					getUserConnectionRegistrationStruct(void);

        // Socket connection and user registration for "socketManager.cpp"
        int                                     verifyClientAndServerResponse(struct pollfd fds, bot *bot);
        int                                     acceptNewConnection(struct pollfd *fds, int nfds, bot *bot);
        int                                     retrieveDataFromConnectedSocket(int socketID, struct pollfd *fds, bool closeConnection, bot *bot);
        std::string                             createServerResponseForConnection(int socket, Server::userConnectionRegistration *userInfo);
        void                                    createNewUserAtConnection(std::string nickname, std::string username, int socket);
        bool           		                  	getClientInformationsOnConnection(struct pollfd fds, Server::userConnectionRegistration *userInfo, bot *bot);
        bool									findPassInBuffer(char *buffer, Server::userConnectionRegistration *userInfo);
		bool									findNickInBuffer(char *buffer, Server::userConnectionRegistration *userInfo);
		bool									findUserInBuffer(char *buffer, Server::userConnectionRegistration *userInfo);


        // Utility functions
        void                                    startServer();
        void	                                createRandomUsername( User user );
        bool   	                                passwordAlreadyRegistred( void );
        bool   	                                nickAlreadyExist( std::string new_nick );
        bool 	                                channelAlreadyExist(std::string channel_name);
        bool   	                                usernameAlreadyExist( std::string new_username );
		bool	                                userInChannel(std::string channel_name, const User *user);
		bool									channelHaveLimit(std::string channel_name);
		bool									channelEnoughSpace(std::string channel_name);
		bool									channelIsInviteOnly(std::string channel_name);
		void	                                kickUserFromChannel(std::string channel_name, const User *user);
		void									addUserToChannel(std::string channel_name, const User *user, UserAspects channel_aspects);
		void   	                            	createNewChannel(std::string channel_name, Channel *channel);
};

#endif
