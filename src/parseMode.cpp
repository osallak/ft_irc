# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "../include/utils.hpp"

void    Server::parseMode(std::vector<std::string> &vec, int fd)
{
    std::map<std::string,Channel>::iterator  it;
    std::vector<int>                         v;
    std::string                              message;
    std::vector<int>                         v_cl;
    int                                      i;

    if (vec.size() == 0)
    {
        message = ":" + GetUserName(fd) + " 461 * :Not enough parameters\n";
        sendMessage(fd, message);
        return ;
    }
    //channels mode
    //no such channel
    it = __channels.find(vec[0]);
    if (it == __channels.end())
    {
        message = ":" + GetUserName(fd) + " 403 * :No such channel\n";
        sendMessage(fd, message);
        return ;
    }
    //channel without mode
    if (vec.size() == 1)
    {
        message = ":" + GetUserName(fd) + " 324 * :Not enough parameters\n";
        sendMessage(fd, message);
        return ;
    }
    //channel with modes
    if (vec[1].size() == 2)
    {
        //if it is moderator
        v = it->second.getChannelModerator();
        if (std::find(v.begin(), v.end(), fd) != v.end())
        {
            //fd of client
            i = it->second.getChannelClient(vec[2]);
            //plus or minus mode 
            if (vec[1][0] == PLUS)
            {
                if (vec[1][1] == INV)
                {
                    if (it->second.getChannelType() == 1)
                    {
                        message = ":" + GetUserName(fd) + " 342 * :Channel is already private\n";
                        sendMessage(fd, message);
                        return ;
                    }
                    it->second.setChannelType(1);
                    message = ":" + GetUserName(fd) + " 342 * :Channel is private now\n";
                    sendMessage(fd, message);
                    return ;
                }
                if (vec[1][1] == KEY)
                {
                    if (i != -1)
                    {
                        message = ":" + GetUserName(fd) + " 401 * :No such nick/channel\n";
                        sendMessage(fd, message);
                        return ;
                    }
                    if (vec.size() == 3)
                    {
                        it->second.setChannelPass(1);
                        it->second.setChannelPassword(vec[2]);
                        message = ":" + GetUserName(fd) + " 400 * :We have set the password\n";
                        sendMessage(fd, message);
                        return ;
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        sendMessage(fd, message);
                        return ;
                    }
                }
                if (vec[1][1] == MOD)
                {
                    if (vec.size() == 3)
                    {
                        if (i != -1)
                        {
                            if (IsModerator(it->second, vec[2]) == 1)
                            {
                                message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is already a moderator\n";
                                sendMessage(fd, message);
                                return ;
                            }
                            it->second.setChannelModerator(i);
                            message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is a moderator now\n";
                            sendMessage(fd, message);
                            return ;
                        }
                        else
                        {
                            message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is not a member of channel\n";
                            sendMessage(fd, message);
                            return ;
                        }
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        sendMessage(fd, message);
                        return ;
                    }
                }
            }
            else if (vec[1][0] == MINUS)
            {
                if (vec[1][1] == INV)
                {
                    if (it->second.getChannelType() == 0)
                    {
                        message = ":" + GetUserName(fd) + " 400 * :" + "channel " + it->second.getChannelName() + " is already public\n";
                        sendMessage(fd, message);
                        return ;
                    }
                    it->second.setChannelType(0);
                    message = ":" + GetUserName(fd) + " 400 * :" + "channel " + it->second.getChannelName() + " is public now\n";
                    sendMessage(fd, message);
                    return ;
                }
                if (vec[1][1] == KEY && i != -1)
                {
                    if (vec.size() == 2)
                    {
                        it->second.setChannelPass(0);
                        it->second.setChannelPassword("");
                        message = ":" + GetUserName(fd) + " 400 * :" + "channel " + it->second.getChannelName() + " is without password now\n";
                        sendMessage(fd, message);
                        return ;
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        sendMessage(fd, message);
                        return ;
                    }
                }
                if (vec[1][1] == MOD)
                {
                    if (vec.size() == 3)
                    {
                        i = it->second.getChannelClient(vec[2]);
                        if (i != -1)
                        {
                            if (GetUserId(vec[2]) == fd)
                            {
                                message = ":" + GetUserName(fd) + " 400 * :You can't remove yourself from moderators\n";
                                sendMessage(fd, message);
                                return ;
                            }
                            v_cl = it->second.getChannelModerator();
                            if (IsModerator(it->second, GetUserName(fd)))
                            {
                                RemoveModerator(it->second, vec[2]);
                                message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is no longer a moderator now\n";
                                sendMessage(fd, message);
                                return ;
                            }
                            else
                            {
                                message = ":" + GetUserName(fd) + " 482 * :" + "The user " + vec[2] + " is not a moderator of channel\n";
                                sendMessage(fd, message);
                                return ;
                            }
                        }
                        else
                        {
                            message = ":" + GetUserName(fd) + " 441 * :" + "The user " + vec[2] + " is not a member of channel\n";
                            sendMessage(fd, message);
                            return ;
                        }
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        sendMessage(fd, message);
                        return ;
                    }
                }
            }
            else
            {
                message = ":" + GetUserName(fd) + " 324 * :You have set a mode of channel\n";
                sendMessage(fd, message);
                return ;
            }
        }
        else
        {
            message = ":" + GetUserName(fd) + " 482 * :" + "The user " + vec[2] + " is not a moderator of channel\n";
            sendMessage(fd, message);
            return ;
        }
    }
}
