#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

class Server {

    private :

        int _port;
        int _socketfd;
        int _newSocket;
        ssize_t _valRread;
        socklen_t _servLen;
        socklen_t _clientLen;
        struct sockaddr_in _servAddr;
        struct sockaddr_in _clientAddr;

    public :

        Server();
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        int             getSocketfd(void);
        int             getNewSocket(void);
        ssize_t         getValRead(void);
        int             getPort(void);
        socklen_t       getServLen(void);
        socklen_t*      getClientLen(void);
        sockaddr_in*    getServAddr(void);
        sockaddr_in*    getClientAddr(void);

        void    setSocketfd(int fd);
        void    setNewSocket(int fd);
        void    setValRead(ssize_t value);
        void    setPort(int port);
        void    setServAddr(int port);
};

#endif