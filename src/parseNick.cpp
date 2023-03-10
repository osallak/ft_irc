# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void Server::parseNick(std::vector<std::string>__arg,int __UserId)
{
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    if(!__arg.size())
        __arg.push_back(":");
    if(__arg[0] == ":")
       sendMessage(__UserId,":* 431 * No nickname given\n");
    else if(__users.find(GetUserId(__arg[0])) != __users.end() && __arg[0] != __users[__UserId].getNickname())
       sendMessage(__UserId,":* 433 * :NickName is already on server\n");
    else
        __users.find(__UserId)->second.setNickname(__arg[0]);
}

