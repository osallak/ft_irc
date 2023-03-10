# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void Server::parseInvite(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    if(!__arg.size())
        __arg.push_back(":");
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    for(size_t i = 0 ; i < __arg.size();i++)
        std::cout << "<<" <<__arg[i] << ">>" <<std::endl;
    __arg[__arg.size() - 1] = backslashR(__arg[__arg.size() - 1]);
    if((__arg.size() == 1 && __arg[0] == ":" )|| GetUserId(__arg[0]) == -1)
    {
        if(send(__UserId,":* 404 * :No such nick name\n",28, 0) == -1)
        {
            std::cout << "send() failed\n";
            exit(0);
        }
        return;
    }
    if(__arg.size() == 1 || !__channels[__arg[1]].getClientNb())
    {
        if(send(__UserId,":* 403 * :No such channel\n",26, 0) == -1)
        {
            std::cout << "sned() failed\n";
            exit(0);
        }
        return;
    }
    if(__arg[1][0] != '#')
    {
        if(send(__UserId,":* 476 * :Bad Channel Mask\n",27, 0) == -1)
        {
            std::cout << "send() failed\n";
            exit(0);
        }
        return;
    }
    std::vector<int> vec = __channels.find(__arg[1])->second.getChannelModerator();
    if(__channels.find(__arg[1])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) == vec.end())
        __ValRead = send(__UserId,":* 482 * : You're not an operator on that channel\n",50, 0);
    else
    {
        std::string msg = "";
        msg=":* 341 * :" + __users[__UserId].getUsername() + " " + __arg[0] + " " + __arg[1] + '\n';
        __ValRead = send(__UserId,msg.c_str(),msg.size(), 0);
        __channels[__arg[1]].SetInviteds(GetUserId(__arg[0]), __users[GetUserId(__arg[0])]);
    }
    if(__ValRead == - 1)
    {
        std::cout << "send() failed\n";
        exit(0);
    }
}