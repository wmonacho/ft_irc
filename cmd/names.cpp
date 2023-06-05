#include "cmd.hpp"

bool    cmd::parseNames(std::string str, Server *server)
{
    std::vector<std::string> arg = splitString(str, " ");

    std::map<std::string, Channel*> map = server->getMap();
    std::vector<User> copy_list_user = server->getUserList();

    if (arg.size() == 1)
    {
        for (std::map<std::string, Channel*>::iterator it = map.begin(); it != map.end(); it++)
        {
            //checkez si channel secrete ou pas
            std::cout << "Channel: " << it->first << std::endl;
            std::map<const User*, UserAspects> userlist = it->second->getUserList();
            for (std::map<const User*, UserAspects>::iterator itUser = userlist.begin(); itUser != userlist.end(); itUser++)
            {
                std::cout << itUser->first->getUsername() << std::endl;
                std::vector<User>::iterator iter = std::find(copy_list_user.begin(), copy_list_user.end(), itUser->first);
                if (iter != copy_list_user.end())
                    copy_list_user.erase(iter);
            }
        }
        for (std::vector<User>::iterator cpyIt = copy_list_user.begin(); cpyIt != copy_list_user.end(); cpyIt++)
            std::cout << (*cpyIt).getUsername() << std::endl;
    }
    else
    {
        std::vector<std::string> chan = splitString(arg[1], ",");
        //print les channels passes en parametre ainsi que leurs users respectifs tout en faisant attention
        //a ne pas afficher les utilisateurs qui ne sont pas visibles.
    }
    //petite boucle while pour faire les checks
    return true;
}