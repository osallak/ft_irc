# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void Server::parsePart(std::vector<std::string>__arg,int __UserId)
{
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    if(!__arg.size()) {
        sendMessage(__UserId,":* 461 * :Not enough parameters\n");
        return;
    }
    std::vector<std::string>channels = split(__arg[0],',');
    for(size_t i = 0; i < channels.size();i++)
    {
        if(__channels.find(channels[i]) == __channels.end())
        sendMessage(__UserId,":* 403 * :No such channel\n");
        else if(__channels.find(channels[i])->second.getChannelClients().find(__UserId) ==
             __channels.find(channels[i])->second.getChannelClients().end())
            sendMessage(__UserId,":* 442 * :You're not on that channel\n");
        else if(channels[i][0] != '#')
           sendMessage(__UserId,":* 476 * :Bad Channel Mask\n");
        else
            __channels[channels[i]].eraseClient(__UserId);
    }
}