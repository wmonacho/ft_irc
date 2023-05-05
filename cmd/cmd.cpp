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

std::vector<std::string> cmd::splitString(std::string str, const char *delim)
{
    std::vector<std::string> out;
    tokenize(str, delim, out);
    return (out);
}

bool    cmd::parsePass(std::string str, Server server)
{
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() != 2 || server.alreadyRegistred())
        return (false);
    server.setPassword(splitArg[1]);
    return (true);
}

bool    cmd::parseNick(std::string str, Server server, User user) //recup le User originaire de la commande
{
    std::vector<std::string> splitArg = splitString(str, " ");
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
    if (server.nickAlreadyExist(splitArg[1]))
        return (false);
    user.setNickname(splitArg[1]);
    return (true);
}

bool    cmd::parseUser(std::string str, Server server)
{
    //verifier si le user existe
    std::vector<std::string> splitArg = splitString(str, " ");
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
        User    new_user;

        server.createRandomUsername(new_user);
        std::string real_name;
        for (unsigned int i = 4; i < splitArg.size(); i++)
        {
            real_name += splitArg[i];
        }
        new_user.setRealname(real_name);
        server.setUserList(new_user);
        std::cout << "hello from parseUser, it's working bitch" << std::endl;
    }
    return (true);
}

bool    cmd::parseMode(std::string str, Server server)
{
    std::cout << "Mode cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() < 3)
    {
        //envoyer numeric replies
        std::cerr << "MODE: need more params " << std::endl;
        return (false);
    }
    //check si # devant la cible
    //check si + ou - apres la cible
    (void)server;
    /*      ERR_NEEDMOREPARAMS              ERR_KEYSET
            ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
            ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
            RPL_CHANNELMODEIS
            RPL_BANLIST                     RPL_ENDOFBANLIST
            RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
            RPL_INVITELIST                  RPL_ENDOFINVITELIST
            RPL_UNIQOPIS */
    /*  mode a faire :      +i invite only
                            +m seulent les users admins peuvent parler dans le channel
                            +v donne le droit a un user de parler meme si +m
                            +n empeche les users externe de rejoinde le channel
        ceux ci pas sur :   +l definit une limite de user
                            +k definit un mdp pour le channel

    */
   return (true);
}

bool    cmd::parseQuit(std::string str)
{
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() != 2)
        return false;
    //remplacer ça par un throw d'exception ça serait sympa nan ?
    std::cerr << "QUIT: user has quit IRC: " << arg[1] << std::endl;
    return true;
}
/*
bool    cmd::parseJoin(std::string str)
{

    std::cout << "Join cmd found" << std::endl;
    std::cout << "str: " << str << std::endl;
}
*/
bool    cmd::parsePart(std::string str)
{
    std::vector<std::string> splitArg = splitString(str, " ");
    if (splitArg.size() < 2)
    {
        //numeric reply
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return (false);
    }
    std::vector<std::string> chan = splitString(str, ",");
    std::vector<std::string>::iterator it = chan.begin();
    while (it != chan.end())
    {
        //verifier si le channel existe
        //verifier si l'user est bien dans le channel
        //si l'user est bien dans le channel:
        //- ecrire un message annoncant le depart de l'user
        //- delete l'user de la liste du chan
        it++;
    }
    return true;
}

bool    cmd::parseTopic(std::string str)
{
    std::vector<std::string> arg = splitString(str, " ");
    if (arg.size() < 2)
    {
        std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
        return false;
    }
    //verifier si le channel existe
    //verifier si le client est dans le channel
    if (arg.size() == 2)
    {
        //verifier que le topic existe
        //ensuite verifier si l'user est operateur :
        //- si oui : set le topic
        //- si non : numeric replies + erreur
        std::cout << "a completer willy" << std::endl;
    }
    return true;
}

/*bool    cmd::parseNames(std::string str)
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

void cmd::whichCmd(std::string cmd, std::string str, Server server, User user)
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
             if (parseNick(str, server, user) == false)
            {
                std::cerr << "Usage: NICK [nickname]" << std::endl;
                return ;
            }
            break;

        case 2:
            if (parseUser(str, server) == false)
            {
                std::cerr << "Usage: USER <user> <mode> <unused> <realname>";
                return ;
            }
            break;

        case 3:
            parseMode(str, server);
            break;

        case 4:

            if (parseQuit(str) == false)
            {
                std::cerr << "Usage: QUIT [ <Quit Message> ]" << std::endl;
                return ;
            }
            break;

        /*case 5:
            parseJoin(str);
            break;
        */
        case 6:

            if (parsePart(str) == false)
            {
                std::cerr << "Usage: PART <channel> *( \",\" <channel> ) [ <Part Message> ] " << std::endl;
                return ;
            }
            break;

        case 7:

            if (parseTopic(str) == false)
            {
                std::cerr << "Usage: <channel> [ <topic> ]" << std::endl;
                return ;
            }
            break;

        /*case 8:
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