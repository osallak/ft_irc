# include "../include/utils.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"


void    sendMessage(int fd, std::string msg)
{
    if ( send(fd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error("error: send() failed");
}

std::string backslashR(const std::string& str)
{
    std::string ret;
    for(size_t i = 0 ; i < str.size();i++)
    {
        if(str[i] != '\r')
            ret.push_back(str[i]);
    }
    if(ret[ret.size() - 1]  == '\n')
        return(ret.substr(0, ret.size() - 1));
    else
        return(ret);
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string>    split(std::string &line, char c)
{
    std::vector<std::string>    vec;
    size_t                      pos = 0;
    std::string                 str;

    while ((pos = line.find(c)) != std::string::npos)
    {
        str = line.substr(0, pos);
        vec.push_back(str);
        line.erase(0, pos + 1);
    }
    vec.push_back(line);
    return (vec);
}

void Server::displayWelcomeMessage(int fd) const
{
    int i = __currentNdx;
    std::string msg = ":" + __hostname + " 001 " +  __NewConnections.find(fd)->second.getNickname() +  " :Welcome to the Internet Relay Network " + __NewConnections.find(fd)->second.getNickname() + "!~" + __NewConnections.find(fd)->second.getNickname() + "@" + "127.0.0.1\r\n";
    msg += ":" + __hostname + " 002 " +  __NewConnections.find(fd)->second.getNickname() + " :Your host is " + __hostname + ", running version leet-irc 1.0.0\r\n";
    msg += ":" + __hostname + " 003 " +  __NewConnections.find(fd)->second.getNickname() + " :This server has been started Wed Oct 12 2022\r\n";
    msg += ":" + __hostname + " 004 " +  __NewConnections.find(fd)->second.getNickname() + " " + __hostname + " leet-irc 1.0.0 aioOrsw aovimntklbeI\r\n";
    msg += ":" + __hostname + " 251 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :There are " +  std::to_string(__users.size() + 1) + " users and 1 bot on 1 servers\r\n";
    msg += ":" + __hostname + " 375 " + __NewConnections.find(fd)->second.getNickname() + " :- " + __hostname + " Message of the day -\r\n";
    msg += ":" + __hostname + " 376 " + __NewConnections.find(fd)->second.getNickname() + " :End of MOTD command\r\n";
    sendMessage(fd,msg);
}
