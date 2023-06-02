#ifndef CMD_HPP
#define CMD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "../server/server.hpp"

class Server;

class cmd
{
private:

    std::string _cmd[14];

public:

    cmd();
    cmd(const cmd &rhs);
    cmd &operator=(const cmd &rhs);
    ~cmd();

    void    whichCmd(std::string str, Server *server, User *user);
    bool    parsePass(User *user, std::string str, Server *server);
    bool    parseNick(std::string str, Server *server, User *user);
    bool    parseUser(std::string str, Server *server);
    bool    parsePart(std::string str, Server *server, User *user);
    bool    parseTopic(std::string str, Server *server, User *user);
    bool    parseQuit(std::string str, User *user);
    bool    parseMode(std::string str, Server *server, User *user);
    bool    parseNames(std::string str, Server *server);
    bool    parseList(std::string str, Server *server);
    bool    parseJoin(std::string str, Server *server, User *user);
    bool    parseKick(std::string str, Server *server, User *user);
    bool    parseInvite(std::string str, Server *server, User *user);
    bool    parsePrivmsg(std::string str, Server *server, User *user);
    std::vector<std::string> splitString(std::string str, const char *delim);
    std::string    createServerMessage(User *user, std::string numReply, std::vector<std::string> splitArg);
    void            sendResponseToAllUsersInChannel(std::string message, Channel *channel);
    void            sendChannelTopicToUser(Channel *channel, User *user);
};

#endif