#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

class Server {

    private :

        int _socketfd;
        int _newSocket;
        int _valRread;
        int _port;
        struct sockaddr_in servAddr, clientAddr;
        socklen_t servLen, clientLen;

    public :

        Server();
        Server(const Server& obj);
        Server& operator=(const Server& obj);
        ~Server();

        int     getSocketfd(void);
        int     getNewSocket(void);
        int     getAddressLen(void);
        int     getValRead(void);
        int     getPort(void);

        void    setSocketfd(int fd);
        void    setNewSocket(int fd);
        void    setAddressLen(int length);
        void    setValRead(int value);
        void    setPort(int port);
};

#endif