#include "cmd.hpp"
#include "../server/server.hpp"

cmd::cmd()
{
    _cmd[0] = "PASS";
    _cmd[1] = "NICK";
    _cmd[2] = "USER";
    _cmd[3] = "MODE";
    _cmd[4] = "QUIT";
    _cmd[5] = "JOIN";
    _cmd[6] = "PART";
    _cmd[7] = "TOPIC";
    _cmd[8] = "NAMES";
    _cmd[9] = "LIST";
    _cmd[10] = "INVITE";
    _cmd[11] = "KICK";
    _cmd[12] = "PRIVMSG";
    _cmd[13] = "NOTICE";
}

cmd::cmd(const cmd &rhs)
{
    *this = rhs;
}

cmd &cmd::operator=(const cmd &rhs)
{
    _cmd[0] = rhs._cmd[0];
    _cmd[1] = rhs._cmd[1];
    _cmd[2] = rhs._cmd[2];
    _cmd[3] = rhs._cmd[3];
    _cmd[4] = rhs._cmd[4];
    _cmd[5] = rhs._cmd[5];
    _cmd[6] = rhs._cmd[6];
    _cmd[7] = rhs._cmd[7];
    _cmd[8] = rhs._cmd[8];
    _cmd[9] = rhs._cmd[9];
    _cmd[10] = rhs._cmd[10];
    _cmd[11] = rhs._cmd[11];
    _cmd[12] = rhs._cmd[12];
    _cmd[13] = rhs._cmd[13];
    return (*this);
}

cmd::~cmd()
{

}

void tokenize(std::string const &str, const char* delim, std::vector<std::string> &out)
{
    char *token = strtok(const_cast<char*>(str.c_str()), delim);
    while (token != NULL)
    {
        out.push_back(std::string(token));
        token = strtok(NULL, delim);
    }
}

std::vector<std::string> cmd::splitString(std::string str)
{
    const char* delim = " ";

    std::vector<std::string> out;
    tokenize(str, delim, out);
    return (out);
}

bool    cmd::parsePass(std::string str, Server server)
{
    std::vector<std::string> splitArg = splitString(str);
    if (splitArg.size() != 2)
        return (false);
    //check si l'user a deja set son password
    //set son password
    return (true);
}

bool    cmd::parseNick(std::string str)
{
    std::vector<std::string> splitArg = splitString(str);
    if (splitArg.size() != 2)
        return (false);
    int i = 0;

    while (splitArg[1][i])
    {
        if (splitArg[1][i] == '/' || splitArg[1][i] == '_' || splitArg[1][i] == '|'|| \
            splitArg[1][i] == '\\' || splitArg[1][i] == '@')
        {
            //numeric_replies
            std::cerr << "Erroneus_nickname" << std::endl;
            return (false);
        }
        i++;
    }
    //verifier que le nick est valide
    //verifier si le nickname existe deja
    //set le nickname
    return (true);
}

bool    cmd::parseUser(std::string str)
{
    //verifier si le user existe
    std::vector<std::string> splitArg = splitString(str);
    std::cout << "splitArg.size() == " << splitArg.size() << std::endl;
    if (splitArg.size() < 5)
    {
        //envoyer numeric replies
        std::cerr << "USER: not enough parameters" << std::endl;
        return (false);
    }
    if (splitArg.size() > 4 && (splitArg[2] == "0" && splitArg[3] == "*"))
    {
        //setuser
        //setrealname splitArg[4] et + si il y a
        std::cout << "hello from parseUser, it's working bitch" << std::endl;
    }
    return (true);
}

/*bool    cmd::parseMode(std::string str)
{
    std::cout << "Mode cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseQuit(std::string str)
{
    std::cout << "Quit cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseJoin(std::string str)
{
    std::cout << "Join cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parsePart(std::string str)
{
    std::cout << "Part cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseTopic(std::string str)
{
    std::cout << "Topic cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;

}

bool    cmd::parseNames(std::string str)
{
    std::cout << "Names cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;

}

bool    cmd::parseList(std::string str)
{
    std::cout << "List cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseInvite(std::string str)
{
    std::cout << "Invite cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseKick(std::string str)
{
    std::cout << "Kick cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parsePrivmsg(std::string str)
{
    std::cout << "Privmsg cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}

bool    cmd::parseNotice(std::string str)
{
    std::cout << "Notice cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}*/

void cmd::whichCmd(std::string cmd, std::string str, Server server)
{
    int j = -1;
    for (int i = 0; i < 14; i++)
    {
        if (cmd == _cmd[i])
        {
            j = i;
            break;
        }
    }
    switch (j)
    {
        case -1:
            std::cerr << "str: \"" << str << "\" cmd not found" << std::endl;
            return ;

        case 0:
            if (parsePass(str, server) == false)
            {
                std::cerr << "Usage: PASS [password]" << std::endl;
                return ;
            }
            break;

        case 1:
             if (parseNick(str) == false)
            {
                std::cerr << "Usage: NICK [nickname]" << std::endl;
                return ;
            }
            break;

        case 2:
            if (parseUser(str) == false)
            {
                std::cerr << "Usage: USER <user> <mode> <unused> <realname>";
                return ;
            }
            break;

        /*case 3:
            parseMode(str);
            break;

        case 4:
            parseQuit(str);
            break;

        case 5:
            parseJoin(str);
            break;

        case 6:
            parsePart(str);
            break;

        case 7:
            parseTopic(str);
            break;

        case 8:
            parseNames(str);
            break;

        case 9:
            parseList(str);
            break;

        case 10:
             parseInvite(str);
             break;

        case 11:
            parseKick(str);
            break;

        case 12:
            parsePrivmsg(str);
            break;

        case 13:
            parseNotice(str);
            break;*/
    }
}