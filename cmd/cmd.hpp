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

    void    whichCmd(std::string cmd, std::string str, Server server, User user);
    bool    parsePass(std::string str, Server server);
    bool    parseNick(std::string str, Server server, User user);
    bool    parseUser(std::string str, Server server);
    bool    parsePart(std::string str);
    bool    parseTopic(std::string str);
    bool    parseQuit(std::string str);
    bool    parseMode(std::string str, Server server);
    /*bool    parseJoin(std::string str);
    bool    parseNames(std::string str);
    bool    parseList(std::string str);
    bool    parseInvite(std::string str);
    bool    parseKick(std::string str);
    bool    parsePrivmsg(std::string str);
    bool    parseNotice(std::string str);*/
    std::vector<std::string> splitString(std::string str, const char *delim);
};

#endif