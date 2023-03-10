# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void    Server::SetUserInf(std::pair<std::string,std::string> cmd, int UserId)
{
    if(cmd.first != "Error")
    {
            if(__NewConnections.find(UserId)->second.getPassword().empty())
            {
                if(cmd.first == "pass")
                {
                    if(cmd.second == __password  && __NewConnections.find(UserId)->second.getPassword().empty()) {
                        __NewConnections.find(UserId)->second.setPassword(cmd.second);
                    } else {
                        sendMessage(UserId,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n");
                    }
                    return;
                }
                else
                {
                    sendMessage(UserId,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n");
                    return;
                }

            }
            if(cmd.first == "nick") {
                if(cmd.second.empty()) {
                    sendMessage(UserId,":* 461 * Not enough parameters\n");
                    return;
                }
                if(__users.find(GetUserId(cmd.second)) != __users.end()) {
                    sendMessage(UserId,":* 433 * :NickName is already on server\n");
                    return;
                }
                if(!__NewConnections.find(UserId)->second.getPassword().empty() && !__NewConnections.find(UserId)->second.getUsername().empty()) {
                     __NewConnections.find(UserId)->second.setNickname(cmd.second);
                    displayWelcomeMessage(UserId);
                    __NewConnections.find(UserId)->second.setIsLogged(true);
                    __users[UserId] = __NewConnections.find(UserId)->second;
                } else
                    __NewConnections.find(UserId)->second.setNickname(cmd.second);

            }
            if(cmd.first == "user") {
                std::vector<std::string>arg = split(cmd.second,' ');
                if(arg.size() != 4) {
                    sendMessage(UserId,":* 461 * Not enough parameters\n");
                    return;
                }
                cmd.second = arg[0];
                if(cmd.second.empty()) {
                    sendMessage(UserId,":* 461 * Not enough parameters\n");
                    return;
                } else if(__users.find(GetUserId(cmd.second)) != __users.end()) {
                    sendMessage(UserId,":* 462 * You mat not reregister\n");
                    return;
                }
                if(!__NewConnections.find(UserId)->second.getPassword().empty() && !__NewConnections.find(UserId)->second.getNickname().empty())
                {
                    __NewConnections.find(UserId)->second.setUsername(cmd.second);
                    displayWelcomeMessage(UserId);
                    __NewConnections.find(UserId)->second.setIsLogged(true);
                    __users[UserId] = __NewConnections.find(UserId)->second;
                }
                else
                    __NewConnections.find(UserId)->second.setUsername(cmd.second);

            }
    } else {
        sendMessage(UserId,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n");
    }
}

std::vector<std::pair<std::string, std::string> > Server::ParceConnectionLine(std::string cmd)
{
    cmd = trim(cmd);
    cmd = backslashR(cmd);
    std::vector<std::pair<std::string,std::string> >ret;
    std::size_t __found  = cmd.find("\n");
    std::vector<std::string>ConnectionInf(3);
    ConnectionInf[0] =  cmd.substr(0,__found);
    cmd = cmd.substr(__found + 1 ,- 1);
    __found =  cmd.find("\n");
    ConnectionInf[1] = cmd.substr(0,__found);
    ConnectionInf[2] = cmd.substr(__found + 1 , -1);
    for(unsigned int i = 0 ; i < 3;i++)
        ConnectionInf[i] = trim( ConnectionInf[i]);
    for(int i = 0 ; i < 3;i++)
    {
        __found = ConnectionInf[i].find(" ");
        if(__found == std::string::npos)
            return(ret);
        ret.push_back(std::make_pair(trim(ConnectionInf[i].substr(0,__found)),trim(ConnectionInf[i].substr(__found + 1, -1))));
    }
    size_t fnd = ret[0].second.find(':');
    if (fnd != std::string::npos)
        ret[0].second.erase(ret[0].second.begin() + fnd);
    fnd = ret[1].second.find(':');
    if (fnd != std::string::npos)
        ret[1].second.erase(ret[1].second.begin() + fnd);
    if(ret[0].first == "user")
    {
        if(ret[2].first != "nick" || ret[1].first != "nick")
            ret.push_back(std::make_pair("error","error"));
    }
    else if((ret[0].first != "pass" || ret[0].second != __password ) || ret[2].first != "nick" || ret[1].first != "user")
    {
        if(ret[2].first != "nick" || ret[1].first != "nick")
            ret.push_back(std::make_pair("error","error"));
    }
    else if((ret[0].first != "pass" || ret[0].second != __password ) || ret[2].first != "nick" || ret[1].first != "user")
        ret.push_back(std::make_pair("error","error"));
    return(ret);
}

std::pair<std::string,std::string> Server::ParceConnection(std::string cmd)
{
    cmd = trim(cmd);
    std::size_t found = cmd.find(" ");
    std::pair<std::string,std::string>ret;
    if(found == std::string::npos)
    {
        ret.first = "Error";
        ret.second = "Error";
    }
    else
    {
        ret.first = cmd.substr(0,found);
        ret.second = cmd.substr(found + 1, cmd.size() - found - 1);
    }
    ret.first = trim(ret.first);
    ret .second = trim(ret.second);
    if(ret.first != "pass" && ret.first != "user" && ret.first != "nick")
    {
        ret.first = "Error";
        ret.second = "Error";
    }
    return(ret);
}