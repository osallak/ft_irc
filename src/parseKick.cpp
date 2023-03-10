# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"


void    Server::parseKick(std::vector<std::string> &vec, int fd)
{
    std::string message;
    if (vec.size() == 0) {
        message = ":" + __users[fd].getNickname() + " 461 " + "KICK " + "Not enough parameters\n";
        sendMessage(fd, message);
        return ;
    } else if (vec.size() > 3){
        message = ":" + __users[fd].getNickname() + " 461 " + "KICK " + "Too many parameters\n";
        sendMessage(fd, message);
        return ;
    }
    std::map<std::string, Channel>::iterator it = __channels.find(vec[0]);
    if (it == __channels.end()) {
        message = ":" + __users[fd].getNickname() + " 403 " + "KICK " + "No such channel\n";
        sendMessage(fd, message);
        return ;
    }
    Channel channel = it->second;
    if (!isInChannel(channel, fd))
    {
        message = ":" + __users[fd].getNickname() + " 442 " + "KICK " + "You're not on that channel\n";
        sendMessage(fd, message);
        return ;
    }
    std::vector<int>::iterator it3 = std::find(channel.getChannelModerator().begin(), channel.getChannelModerator().end(), fd);
    if (it3 == channel.getChannelModerator().end())
    {
        message = ":" + __users[fd].getNickname() + " 482 " + "KICK " + "You're not a channel operator\n";
        sendMessage(fd, message);
        return ;
    }
    std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
    for (; it2 != channel.getChannelClients().end(); ++it2)
    {
        if (it2->second.getNickname() == vec[1] && it2->first != fd)
        {
            message = ":" + __users[fd].getNickname() + " KICK " + channel.getChannelName() + " " + vec[1] + " :" + "Kicked by " + __users[fd].getNickname() + "\n";
            sendMessage(it2->first, message);
            channel.eraseClient(it2->first);
            return ;
        }
    }
    message = ":" + __users[fd].getNickname() + " 441 " + "KICK " + "They aren't on that channel\n";
    sendMessage(fd, message);
}