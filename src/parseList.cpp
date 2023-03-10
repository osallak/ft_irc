# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"


void    Server::parseList(std::vector<std::string> &vec, int fd)
{
    std::string message;
    if (vec.size() > MAXPARAMS)
    {
        message = ":" + __users[fd].getNickname() + " 461 " + "LIST " + "Too many arguments\n";
        sendMessage(fd, message);
        return ;
    }
    if (vec.size() == 0)
    {
        std::map<std::string, Channel>::iterator it = __channels.begin();
        message = ":" + __users[fd].getNickname() + " 321 " + "LIST " + "Channel :Users Name\n";
        sendMessage(fd, message);
        for (; it != __channels.end(); ++it)
        {
            if (it->second.getChannelType() == 1 && !isInChannel(it->second, fd))
                continue;
            message = ":" + __users[fd].getNickname() + " 322 " + it->second.getChannelName() + " " + std::to_string(it->second.getChannelClients().size()) + " :" + it->second.getChannelTopic() + "\n";
            sendMessage(fd, message);
        }
        sendMessage(fd, "323 End of /LIST\n");
        return ;
    }
    
    std::vector<std::string> chns = split(vec[0], ',');

    for (size_t i = 0; i < chns.size(); ++i) {
        std::map<std::string, Channel>::iterator it = __channels.find(chns[i]);
        if (it == __channels.end()) {
            message = ":" + __users[fd].getNickname() + " 403 " + "LIST " + "No such channel\n";
            sendMessage(fd, message);
            return ;
        }
        Channel channel = it->second;
        if (channel.getChannelType() == 1 && isInChannel(channel, fd) == false)
        {
            message = ":" + __users[fd].getNickname() + " 404 " + "LIST " + "Cannot send to channel\n";
            sendMessage(fd, message);
            return ;
        }
        message = ":" + __users[fd].getNickname() + " 322 " + channel.getChannelName() + " " + std::to_string(channel.getChannelClients().size()) + " :" + channel.getChannelTopic() + "\n";
        sendMessage(fd, message);
    }
}