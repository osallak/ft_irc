# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void Server::parseInvite(std::vector<std::string>__arg,int __UserId)
{
    if(!__arg.size())
        __arg.push_back(":");
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    if((__arg.size() == 1 && __arg[0] == ":" )|| GetUserId(__arg[0]) == -1)
    {
        sendMessage(__UserId,":* 404 * :No such nick name\n");
        return;
    }
    if(__arg.size() == 1 || !__channels[__arg[1]].getClientNb())
    {
        sendMessage(__UserId,":* 403 * :No such channel\n");
        return;
    }
    if(__arg[1][0] != '#')
    {
        sendMessage(__UserId,":* 476 * :Bad Channel Mask\n");
        return;
    }
    std::vector<int> vec = __channels.find(__arg[1])->second.getChannelModerator();
    if(__channels.find(__arg[1])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) == vec.end())
        sendMessage(__UserId,":* 482 * : You're not an operator on that channel\n");
    else
    {
        std::string msg = "";
        msg=":* 341 * :" + __users[__UserId].getNicknameById() + " invite " + __arg[0] + " to " + __arg[1] + '\n';
        sendMessage(__UserId,msg);
        __channels[__arg[1]].SetInviteds(GetUserId(__arg[0]), __users[GetUserId(__arg[0])]);
    }
}