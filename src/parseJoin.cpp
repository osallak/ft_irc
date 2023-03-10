
# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void    Server::parseJoin(std::vector<std::string> &vec, int fd)
{
    std::vector<std::string>                    chn;
    std::vector<std::string>                    key;
    std::map<std::string,Channel>::iterator     it;
    bool                                         inv;
    std::string                                 str;
    std::string                                 message;
    size_t                                      i;
    size_t                                      k;

    k = 0;
    if (vec.size() == 0 || vec[0][0] != '#' || (vec[0][0] == '#' && vec[0].size() == 1))
    {
        message = ":" + GetUserName(fd) + " 461 JOIN :Not enough parameters\n";
        sendMessage(fd, message);
        return ;
    }
    chn = split(vec[0], ',');
    if (vec.size() == 2)
        key = split(vec[1], ',');
    //check if channels exist
    for (i = 0;i < chn.size(); ++i)
    {
        while (1)
        {
            it = __channels.find(chn[i]);
            if (it == __channels.end())
            {
                __channels[chn[i]].setChannelName(chn[i]);
                __channels[chn[i]].setChannelTopic("No topic");
                __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                __channels[chn[i]].setChannelModerator(fd);
                __channels[chn[i]].setChannelType(0);
                __channels[chn[i]].setChannelPass(0);
                __channels[chn[i]].setChannelPassword("");
                if (vec.size() == 2 && key.size() > i)
                {

                    __channels[chn[i]].setChannelType(1);
                    __channels[chn[i]].setChannelPass(1);
                    __channels[chn[i]].setChannelPassword(key[i]);
                }
                message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                sendMessage(fd, message);
                break ;
            }
            //if user already exist
            if (it->second.getChannelClientt(fd) == 1)
            {
                message = ":" + GetUserName(fd) + " 443 * :You are already in this channel\n";
                sendMessage(fd, message);
                break ;
            }
            //check if it's private
            if (it->second.getChannelType() == 1)
            {
                if (it->second.getChannelInvited().empty() && vec.size() != 2)
                {
                    message = ":" + GetUserName(fd) + " 473 * Cannot join channel (+i)\n";
                    sendMessage(fd, message);
                    break ;
                }
                inv = it->second.getInvited(fd);
                if (inv && vec.size() != 2)
                {
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                    message = ":" + GetUserName(fd) + " JOIN " + chn[i] + "\n";
                    sendMessage(fd, message);
                    break ;
                }
                if (it->second.getChannelPass() == 1)
                {
                    if ((k < key.size() && it->second.getChannelPassword() != key[k++]) || key.size() == 0)
                    {
                        message = ":" + GetUserName(fd) + " 475 * Cannot join channel (+k)\n";
                        sendMessage(fd, message);
                        break ;
                    }
                    else
                    {
                        __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                        message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                        sendMessage(fd, message);
                        break ;
                    }
                }
                else
                {
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                    message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                    sendMessage(fd, message);
                    break ;
                }
            }
            else
            {
                if (it->second.getChannelPass() == 1)
                {
                    if (key.size() <= i || it->second.getChannelPassword() != key[k++])
                    {
                        message = ":" + GetUserName(fd) + " 475 * Cannot join channel (+k)\n";
                        sendMessage(fd, message);
                        break ;
                    }
                    else
                    {
                        __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                        message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                        sendMessage(fd, message);
                        break ;
                    }
                }
                else  
                {
                    if (it->second.getChannelClientt(fd) == 1)
                    {
                        message = ":" + GetUserName(fd) + " 443 * :You are already in this channel\n";
                        sendMessage(fd, message);
                        break ;
                    }
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                    message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                    sendMessage(fd, message);
                    break ;
                }
            }
        }
    }
}