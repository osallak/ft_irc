# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void Server::parseTopic(std::vector<std::string>__arg,int __UserId)
{
    if(!__arg.size() || __arg.size() == 1 || __channels.find(__arg[0]) == __channels.end())
    {
            sendMessage(__UserId,":* 403 * :No such channel\n");
            return;
    }
    if(__arg[0][0] != '#')
    {
       sendMessage(__UserId,":* 476 * :Bad Channel Mask\n");
        return;
    }
    std::vector<int> vec = __channels.find(__arg[0])->second.getChannelModerator();
    if(__channels.find(__arg[0])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) == vec.end())
        sendMessage(__UserId,":* 482 * You're not an operator on that channel\n");

    else
    {
        std::map<int, Client>::const_iterator BeginIt;
        std::map<int, Client>::const_iterator EndIt;
        BeginIt = __channels.find(__arg[0])->second.BigenIterator();
        EndIt = __channels.find(__arg[0])->second.EndIterator();
        while(BeginIt != EndIt)
        {
             __channels.find(__arg[0])->second.setChannelTopic(__arg[1]);
            std::string msg = ": TOPIC 332 :"+__channels.find(__arg[0])->second.getChannelName() + " " + __channels.find(__arg[0])->second.getChannelTopic() + "\n";
            sendMessage(BeginIt->first, msg);
            BeginIt++;
        }
    }
}