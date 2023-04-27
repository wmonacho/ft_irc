#include "server.hpp"

// *** CONSTRUCTORS AND DESTRUCTOR ***

Server::Server() {}

Server::Server(const Server& obj) : _socketfd(obj._socketfd), _newSocket(obj._newSocket), _addressLen(obj._addressLen), _valRread(obj._valRread) {

    *this = obj;
    return ;
}

Server& Server::operator=(const Server& obj) {

    *this = obj;
    return *this;
}

Server::~Server() {}

// *** GETTERS AND SETTERS ***

int Server::getSocketfd(void) {

    return this->_socketfd;
}

int Server::getNewSocket(void) {

    return this->_newSocket;
}

int Server::getAddressLen(void) {

    return this->_addressLen;
}

int Server::getValRead(void) {

    return this->_valRread;
}

int Server::getPort(void) {

    return this->_port;
}

void    Server::setSocketfd(int fd) {

    this->_socketfd = fd;
}

void    Server::setNewSocket(int fd) {

    this->_newSocket = fd;
}

void    Server::setAddressLen(int length) {

    this->_addressLen = length;
}

void    Server::setValRead(int value) {

    this->_valRread = value;
}

void    Server::setPort(int port) {

    this->_port = port;
}