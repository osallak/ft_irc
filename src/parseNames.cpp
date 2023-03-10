# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"


void    Server::parseNames(std::vector<std::string> &vec, int fd)
{
    std::string message;
    if (vec.size() == 0)
    {
        std::map<int, Client>::const_iterator it = __users.begin();
        message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + "Users\n";
        sendMessage(fd, message);
        for (; it != __users.end(); ++it)
        {
            message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + it->second.getNicknameById() + "\n";
            sendMessage(fd, message);
        }
        message = ":" + __users[fd].getNickname() + " 366 " + "NAMES " + "= " + "End of /NAMES list\n";
        sendMessage(fd, message);
        return ;
    }
    if (vec.size() > MAXPARAMS)
    {
        message = ":" + __users[fd].getNickname() + " 461 " + "NAMES " + "Too many arguments\n";
        sendMessage(fd, message);
        return ;
    }
    std::vector<std::string> targetsVec = split(vec[0], ',');
    for (size_t i = 0; i < targetsVec.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = __channels.find(targetsVec[i]);

        if (it == __channels.end()) {
            message = ":" + __users[fd].getNickname() + " 403 " + "NAMES " + "No such channel\n";
            sendMessage(fd, message);
            return;
        }
        Channel channel = it->second;
        if (isInChannel(channel, fd) == false)
        {
            message = ":" + __users[fd].getNickname() + " 404 " + "NAMES " + "Cannot send to channel\n";
            sendMessage(fd, message);
            return ;
        }
        std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
        message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + channel.getChannelName() + "\n";
        sendMessage(fd, message);
        for (; it2 != channel.getChannelClients().end(); ++it2)
        {
            message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + it2->second.getNicknameById() + "\n";
            sendMessage(fd, message);
        }
    }
}