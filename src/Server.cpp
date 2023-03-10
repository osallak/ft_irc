# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "../include/utils.hpp"
/* 
    - every attribute prefixed with __ is private
    - every attribute prefixed with _ is protected
    - every attribute prefixed with nothing is public

    - getters should be const
    - do not change something if some of your team members are working on it
    - make a todo list of what you want to do
    - do not push to main
    - make a local branch for each feature you want to add, bug you want to fix, etc...

*/
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
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
Server::Server() : __port(-1), __password("")
{
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    this->__hostname = hostname;
}

Server::~Server()
{
    //close sockets and stuff etc...
}

Server::Server(const Server &copy)
{
    *this = copy;
}

Server &Server::operator=(const Server &copy)
{
    if (this != &copy)
    {
        this->__port = copy.__port;
        this->__password = copy.__password;
    }
    return (*this);
}
void    Server::SetUserInf(std::pair<std::string,std::string> cmd, int UserId)
{
    int __ValRead = 0;
    if(cmd.first != "Error")
    {
            if(__NewConnections.find(UserId)->second.getPassword().empty())
            {
                if(cmd.first == "pass")
                {
                    if(cmd.second == __password  && __NewConnections.find(UserId)->second.getPassword().empty())
                        __NewConnections.find(UserId)->second.setPassword(cmd.second);
                    else
                    {
                        if(send(UserId,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n",62,0) == -1)
                        {
                            std::cout << "Error: send failed" << std::endl;
                            exit(0);
                        }
                    }
                    return;
                }
                else
                {
                    if(send(UserId,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n",62,0) == -1)
                    {
                        std::cout << "Error: send failed" << std::endl;
                        exit(0);
                    }
                    return;
                }

            }
            if(cmd.first == "nick")
            {
                if(cmd.second.empty())
                {
                    if(send(UserId,":* 461 * Not enough parameters\n",32,0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    return;
                }
                if(__users.find(GetUserId(cmd.second)) != __users.end())
                {
                    std::cout << "|" << cmd.second << "|" << std::endl;
                    if((__ValRead = send(UserId,":* 433 * :NickName is already on server\n",40,0)) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    return;
                }
                if(!__NewConnections.find(UserId)->second.getPassword().empty() && !__NewConnections.find(UserId)->second.getUsername().empty())
                {
                    __NewConnections.find(UserId)->second.setNickname(cmd.second);
                    std::string msg = ":" + std::string("irc.1337.ma") + " 001 " +  __NewConnections.find(UserId)->second.getNickname() +  " :Welcome to the Internet Relay Network " + __NewConnections.find(UserId)->second.getNickname() + "!~" + "yahya" + "@" + "127.0.0.1" + "\r\n";;
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":" + std::string("irc.1337.ma") + " 002 " +  __NewConnections.find(UserId)->second.getNickname() + " :Your host is " + "irc.1337.ma" + ", running version 1.0\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":" + std::string("irc.1337.ma") + " 003 " +  __NewConnections.find(UserId)->second.getNickname() + " :This server was created Wed Oct 12 2022 at 20:31:55 UTC\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    // msg = ":" + std::string("irc.1337.ma") + " 004 " +  __NewConnections.find(UserId)->second.getNickname() + " " + __hostname + " 1.0\r\n";
                    msg = ":" + std::string("irc.1337.ma") + " 004 " +  __NewConnections.find(UserId)->second.getNickname() + " irc.1337.ma leet-irc 1.0.0 aioOrsw aovimntklbeI\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":irc.1337.ma 251 " +  __NewConnections.find(UserId)->second.getNickname() + " :There are 2 users and 0 services on 1 servers\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":irc.1337.ma 375 " + __NewConnections.find(UserId)->second.getNickname() + " :- irc.1337.ma Message of the day -\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":irc.1337.ma 376 " + __NewConnections.find(UserId)->second.getNickname() + " :End of MOTD command\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    __NewConnections.find(UserId)->second.setIsLogged(true);
                    __users[UserId] = __NewConnections.find(UserId)->second;
                }
                else
                    __NewConnections.find(UserId)->second.setNickname(cmd.second);

            }
            if(cmd.first == "user")
            {
                std::vector<std::string>arg = split(cmd.second,' ');
                if(arg.size() != 4)
                {
                    if(send(UserId,":* 461 * Not enough parameters\n",32,0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    return;
                }
                cmd.second = arg[0];
                if(cmd.second.empty())
                {
                    if(send(UserId,":* 461 * Not enough parameters\n",32,0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    return;
                }
                else if(__users.find(GetUserId(cmd.second)) != __users.end())
                {
                    if(send(UserId,":* 462 * You mat not reregister\n",32,0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    return;
                }
                if(!__NewConnections.find(UserId)->second.getPassword().empty() && !__NewConnections.find(UserId)->second.getNickname().empty())
                {
                    std::string msg = ":" + std::string("irc.1337.ma") + " 001 " +  __NewConnections.find(UserId)->second.getNickname() +  " :Welcome to the Internet Relay Network " + __NewConnections.find(UserId)->second.getNickname() + "!~" + "yahya" + "@" + "127.0.0.1" + "\r\n";;
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":" + std::string("irc.1337.ma") + " 002 " +  __NewConnections.find(UserId)->second.getNickname() + " :Your host is " + "irc.1337.ma" + ", running version 1.0\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":" + std::string("irc.1337.ma") + " 003 " +  __NewConnections.find(UserId)->second.getNickname() + " :This server was created Wed Oct 12 2022 at 20:31:55 UTC\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    // msg = ":" + std::string("irc.1337.ma") + " 004 " +  __NewConnections.find(UserId)->second.getNickname() + " " + __hostname + " 1.0\r\n";
                    msg = ":" + std::string("irc.1337.ma") + " 004 " +  __NewConnections.find(UserId)->second.getNickname() + " irc.1337.ma leet-irc 1.0.0 aioOrsw aovimntklbeI\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":irc.1337.ma 251 " +  __NewConnections.find(UserId)->second.getNickname() + " :There are 2 users and 0 services on 1 servers\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":irc.1337.ma 375 " + __NewConnections.find(UserId)->second.getNickname() + " :- irc.1337.ma Message of the day -\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    msg = ":irc.1337.ma 376 " + __NewConnections.find(UserId)->second.getNickname() + " :End of MOTD command\r\n";
                    if(send(UserId,msg.c_str(),msg.size(),0) == -1)
                    {
                        std::cout << "send() flaid\n";
                        exit(0);
                    }
                    __NewConnections.find(UserId)->second.setUsername(cmd.second);
                    __NewConnections.find(UserId)->second.setIsLogged(true);
                    __users[UserId] = __NewConnections.find(UserId)->second;
                }
                else
                    __NewConnections.find(UserId)->second.setUsername(cmd.second);

            }
        if(__ValRead == -1)
        {
            std::cout << "send() flaid\n";
            exit(0);
        }
    }
    else
    {
        if(send(UserId,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n",62,0) == -1)
        {
            std::cout << "Error: send failed" << std::endl;
            exit(0);
        }
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

unsigned short Server::getPort() const
{
    return (this->__port);
}

void Server::setPort(std::string port)
{
    this->__port = atoi(port.c_str());
}

int    Server::GetUserId(std::string nick)
{
    std::map<int, Client>::iterator it;
    for (it = __users.begin(); it != __users.end(); ++it) {
        if(it->second.getNickname() == nick)
            return(it->first);
    }
    return -1;
}

void    Server::__ListChannelsUserInvTo(int UserId)
{
    int __ValRead = 0;
    if(send(UserId,"channels can you access\n",25,0) == -1)
    {
        std::cout << "send() Failed\n";
                exit(0);
    }
    std::map<std::string, Channel>::iterator it;
    for (it = __channels.begin(); it != __channels.end(); ++it) {
        if(it->second.getInvited(UserId))
        {
            __ValRead = send(UserId,it->second.getChannelName().c_str(),it->second.getChannelName().size(),0);
            __ValRead = send(UserId,"\n",1,0);
            if(__ValRead == -1)
            {
                std::cout << "send() Failed\n";
                exit(0);
            }
        }
    }
}

void Server::DeleteUser(int __UserId)
{
    std::map<std::string,Channel>::iterator it = __channels.begin();
    while(it != __channels.end())
    {
        it->second.eraseClient(__UserId);
        it->second.eraseInvited(__UserId);
        it++;
    }
    if(__users.find(__UserId) != __users.end())
        __users.erase(__UserId);
    if(__NewConnections.find(__UserId) != __NewConnections.end())
        __NewConnections.erase(__UserId);
     close(__pollfds[__currentNdx].fd);
}
void Server::parseNick(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    if(!__arg.size())
        __arg.push_back(":");
    if(__arg[0] == ":")
        __ValRead = send(__UserId,":* 431 * No nickname given\n",27,0);
    else if(__users.find(GetUserId(__arg[0])) != __users.end() && __arg[0] != __users[__UserId].getNickname())
        __ValRead = send(__UserId,":* 433 * :NickName is already on server\n",40,0);
    else
        __users.find(__UserId)->second.setNickname(__arg[0]);
    if(__ValRead == -1)
    {
        std::cout << "send() faild\n";
        exit(0);
    }
}

void Server::parseQuit(int __UserId)
{
    DeleteUser(__UserId);
}

void Server::parsePart(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    if(!__arg.size())
    {
        if(send(__UserId,":* 461 * :Not enough parameters\n",32, 0) == -1)
        {
            std::cout << "send() Failed\n";
            exit(0);
        }
        return;
    }
    std::vector<std::string>channels = split(__arg[0],',');
    for(size_t i = 0; i < channels.size();i++)
    {
        if(__channels.find(channels[i]) == __channels.end())
        __ValRead = send(__UserId,":* 403 * :No such channel\n",26, 0);
        else if(__channels.find(channels[i])->second.getChannelClients().find(__UserId) ==
             __channels.find(channels[i])->second.getChannelClients().end())
            __ValRead = send(__UserId,":* 442 * :You're not on that channel\n",37, 0);
        else if(channels[i][0] != '#')
            __ValRead = send(__UserId,":* 476 * :Bad Channel Mask\n",27, 0);
        else
            __channels[channels[i]].eraseClient(__UserId);
        if(__ValRead == -1)
        {
            std::cout << "send() failde\n";
            exit(0);
        }
        __ValRead = 0;
    }
}
void Server::parseInvite(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    if(!__arg.size())
        __arg.push_back(":");
    for(size_t i = 0 ; i < __arg.size();i++)
        __arg[i] = backslashR(__arg[i]);
    for(size_t i = 0 ; i < __arg.size();i++)
        std::cout << "<<" <<__arg[i] << ">>" <<std::endl;
    __arg[__arg.size() - 1] = backslashR(__arg[__arg.size() - 1]);
    if((__arg.size() == 1 && __arg[0] == ":" )|| GetUserId(__arg[0]) == -1)
    {
        if(send(__UserId,":* 404 * :No such nick name\n",28, 0) == -1)
        {
            std::cout << "send() failde\n";
            exit(0);
        }
        return;
    }
    if(__arg.size() == 1 || !__channels[__arg[1]].getClientNb())
    {
        if(send(__UserId,":* 403 * :No such channel\n",26, 0) == -1)
        {
            std::cout << "sned() failde\n";
            exit(0);
        }
        return;
    }
    if(__arg[1][0] != '#')
    {
        if(send(__UserId,":* 476 * :Bad Channel Mask\n",27, 0) == -1)
        {
            std::cout << "send() failde\n";
            exit(0);
        }
        return;
    }
    std::vector<int> vec = __channels.find(__arg[1])->second.getChannelModerator();
    if(__channels.find(__arg[1])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) == vec.end())
        __ValRead = send(__UserId,":* 482 * : You're not an operator on that channel\n",50, 0);
    else
    {
        std::string msg = "";
        msg=":* 341 * :" + __users[__UserId].getUsername() + " " + __arg[0] + " " + __arg[1] + '\n';
        __ValRead = send(__UserId,msg.c_str(),msg.size(), 0);
        __channels[__arg[1]].SetInviteds(GetUserId(__arg[0]), __users[GetUserId(__arg[0])]);
    }
    if(__ValRead == - 1)
    {
        std::cout << "send() failde\n";
        exit(0);
    }
}

void Server::parseTopic(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    if(!__arg.size() || __arg.size() == 1 || __channels.find(__arg[0]) == __channels.end())
    {
            __ValRead = send(__UserId,":* 403 * :No such channel\n",26, 0);
            return;
    }
    if(__arg[0][0] != '#')
    {
        __ValRead = send(__UserId,":* 476 * :Bad Channel Mask\n",27, 0);
        return;
    }
    std::vector<int> vec = __channels.find(__arg[0])->second.getChannelModerator();
    if(__channels.find(__arg[0])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) == vec.end())
        __ValRead = send(__UserId,":* 482 * You're not an operator on that channel\n",48, 0);

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
            __ValRead = send(BeginIt->first,msg.c_str(),msg.size(),0);
            BeginIt++;
        }
    }
    if(__ValRead == - 1)
    {
        std::cout << "send() failde\n";
        exit(0);
    }
}

std::string Server::getPassword() const
{
    return (this->__password);
}

void Server::setPassword(std::string password)
{
    if (password.empty())
    {
        std::cerr << "Password cannot be empty" << std::endl;
        exit(1);
    }
    this->__password = password;
}


bool Server::run( void )
{
    //connect to the server, return true if success, false if not
    int __opt = 1;
    
    __socket = socket(AF_INET, SOCK_STREAM, 0);
    if (__socket == -1)
    {
        std::cerr << "Error: socket creation failed" << std::endl;
        return (false);
    }
    if (setsockopt(__socket, SOL_SOCKET, SO_REUSEADDR, &__opt, sizeof(__opt)) < 0)
    {
        std::cerr << "Error: setsockopt failed" << std::endl;
        return (false);
    }
    if (fcntl(__socket, F_SETFD, O_NONBLOCK)  < 0)
    {
        std::cerr << "Error: fcntl failed" << std::endl;
        return (false);
    }
    __address.sin_family = AF_INET;
    __address.sin_addr.s_addr = INADDR_ANY;
    __address.sin_port = htons(__port);
    if (bind(__socket, (struct sockaddr *)&__address, sizeof(__address)) < 0)
    {
        std::cerr << "Error: bind failed" << std::endl;
        return (false);
    }
   
   if (listen(__socket, 3) < 0)
    {
         std::cerr << "Error: listen failed" << std::endl;
         return (false);
    };
    __spollfd.fd = __socket;
    __spollfd.events = POLLIN;
    __spollfd.revents = 0;
    __pollfds.push_back(__spollfd);// add the server socket to the pollfds vector, to keep track of it
    
    // infinite loop to keep the server running

    while (true)
    {
        int ret;
        //pollfd is a struct that contains the file descriptor, the events to check for, and the revents that occured
        // it may be changed if some client disconnects/connects
        // figure out how to add the client sockets to the pollfds vector sumultaneously
        if ( (ret = poll(__pollfds.data(), __pollfds.size(), -1)) < 0)
        {
            std::cerr << "Error: poll failed" << std::endl;
            return (false);
        }
        //loop through the pollfds vector to check which socket has an event

        // from here to the end of the loop, it's not complete yet and it's not working  (some cases are not handled yet)
        for (unsigned int i = 0; i < __pollfds.size(); i++){
            __currentNdx = i;
            if (__pollfds[i].revents == 0) // if there is no event, continue
                continue;
            if (!(__pollfds[i].revents & POLLIN)) // if there is an event but it's not POLLIN, move on to the next socket
                continue;
            if (__pollfds[i].fd == __socket) // if the event is on the server socket, it means a new client is trying to connect
            {
                int new_socket;
                struct sockaddr_in new_address;
                int addrlen = sizeof(new_address);
                if ((new_socket = accept(__socket, (struct sockaddr *)&new_address, (socklen_t*)&addrlen)) < 0)
                {
                    std::cerr << "Error: accept failed" << std::endl;
                    return (false);
                }
                if (fcntl(__socket, F_SETFD, O_NONBLOCK)  < 0) {
                    std::cerr << "Error: fcntl failed" << std::endl;
                    return (false);
                }
                struct pollfd __NewClient;
                __NewClient.fd = new_socket;
                __NewClient.events = POLLIN;
                __pollfds.push_back(__NewClient);
                Client NewClient = Client();
                __NewConnections[__NewClient.fd] = NewClient;
            }
            else 
            {
                int valread;
                char buffer[1024] = {0};
                if ((valread = read(__pollfds[i].fd, buffer, 1024)) < 0)
                {
                    std::cerr << "Error: read failed" << std::endl;
                    return (false);
                }
                std::cout << "the buffer is " << buffer << std::endl;
                if (valread == 0) // if the client disconnected
                {
                    std::cout << "Client disconnected" << std::endl;
                    DeleteUser(__pollfds[i].fd);
                }
                else // this is just for testing, it should be parsed and executed
                {
                    std::string tmpBuffer = buffer;
                    std::cout << "CurrentBuffer : " << buffer << "\n";
                    if(__users.find(__pollfds[i].fd) != __users.end())
                    {
                        std::string CurrentBuffer = __users.find(__pollfds[i].fd)->second.getBuffer();
                        CurrentBuffer+=buffer;
                        __users.find(__pollfds[i].fd)->second.setBuffer(CurrentBuffer);
                        if(CurrentBuffer.find("\n") != std::string::npos)
                        {
                            parseCommand(__pollfds[i].fd);
                            __users.find(__pollfds[i].fd)->second.setBuffer("");
                        }
                    }
                    else
                    {

                        std::string CurrentBuffer = __NewConnections.find(__pollfds[i].fd)->second.getBuffer();
                        CurrentBuffer+=buffer;
                        __NewConnections.find(__pollfds[i].fd)->second.setBuffer(CurrentBuffer);
                        CurrentBuffer =   __NewConnections.find(__pollfds[i].fd)->second.getBuffer();
                        if(CurrentBuffer.find("\n") != std::string::npos)
                        {
                            int __Backtoline = 0;
                            for(unsigned int i = 0 ; i < CurrentBuffer.size();i++)
                            {
                                if(CurrentBuffer[i] == '\n')
                                    __Backtoline++;
                                CurrentBuffer[i] = (char)tolower(CurrentBuffer[i]);
                            }
                            if(__Backtoline == 1)
                            {
                                std::cout << "CurrentBuffer : " << CurrentBuffer << "\n";
                                CurrentBuffer = backslashR(CurrentBuffer);
                                SetUserInf(ParceConnection(CurrentBuffer), __pollfds[i].fd);
                            }
                            else
                            {
                                // std::cout << "current buffer : |" << CurrentBuffer << "|\n";
                                std::vector<std::pair<std::string, std::string> > cmds = ParceConnectionLine(CurrentBuffer);
                                // for (size_t i = 0;i < cmds.size();i++)
                                if(cmds.size() != 3)
                                {
                                    if(send(__pollfds[i].fd,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n",62,0) == -1)
                                    {
                                        std::cout << "Error: send failed" << std::endl;
                                        exit(0);
                                    }
                                }
                                else
                                {
                                    __NewConnections.find(__pollfds[i].fd)->second.setPassword(cmds[0].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setUsername(cmds[1].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setNickname(cmds[2].second);
                                    std::string msg = ":" + std::string("irc.1337.ma") + " 001 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() +  " :Welcome to the Internet Relay Network " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + "!~" + "yahya" + "@" + "127.0.0.1" + "\r\n";;
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    msg = ":" + std::string("irc.1337.ma") + " 002 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :Your host is " + "irc.1337.ma" + ", running version 1.0\r\n";
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    msg = ":" + std::string("irc.1337.ma") + " 003 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :This server was created Wed Oct 12 2022 at 20:31:55 UTC\r\n";
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    // msg = ":" + std::string("irc.1337.ma") + " 004 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " " + __hostname + " 1.0\r\n";
                                    msg = ":" + std::string("irc.1337.ma") + " 004 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " irc.1337.ma leet-irc 1.0.0 aioOrsw aovimntklbeI\r\n";
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    msg = ":irc.1337.ma 251 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :There are 2 users and 0 services on 1 servers\r\n";
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    msg = ":irc.1337.ma 375 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :- irc.1337.ma Message of the day -\r\n";
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    msg = ":irc.1337.ma 376 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :End of MOTD command\r\n";
                                    if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    {
                                        std::cout << "send() flaid\n";
                                        exit(0);
                                    }
                                    __NewConnections.find(__pollfds[i].fd)->second.setIsLogged(true);
                                    __users[__pollfds[i].fd] = __NewConnections.find(__pollfds[i].fd)->second;
                                    // msg = ":irc.1337.ma 251 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :There are 2 users and 0 services on 1 servers\r\n";
                                    // if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    // {
                                    //     std::cout << "send() flaid\n";
                                    //     exit(0);
                                    // }
                                    // msg = ":irc.1337.ma 375 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :- irc.1337.ma Message of the day -\r\n";
                                    // if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    // {
                                    //     std::cout << "send() flaid\n";
                                    //     exit(0);
                                    // }
                                    // msg = ":irc.1337.ma 376 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :End of MOTD command\r\n";
                                    // if(send(__pollfds[i].fd,msg.c_str(),msg.size(),0) == -1)
                                    // {
                                    //     std::cout << "send() flaid\n";
                                    //     exit(0);
                                    // }
                                }
                            }
                            __NewConnections.find(__pollfds[i].fd)->second.setBuffer("");
                        }
                    }
                }
            }
        }
    }
    return (true);
}

void Server::disconnect( void )
{
    //TODO: remove this 
}

int Server::authentification( void )
{
    return (0);
}

void    Server::parseCommand( int fd )
{
    std::string                 line;
    std::string                 command;
    std::vector<std::string>    res;
    size_t                      pos = 0;
    std::string                 str;

    line = __users[fd].getBuffer();
    while ((pos = line.find(' ')) != std::string::npos)
    {
        str = line.substr(0, pos);
        res.push_back(str);
        line.erase(0, pos + 1);

    }
    line = line.substr(0,line.size() - 1);
    res.push_back(line);
    command = res[0];
    for (size_t i = 0; i < command.size(); ++i){
        command[i] = (char)(tolower(command[i]));
    }
    res.erase(res.begin());
    for(size_t i = 0 ; i < res.size();i++)
        std::cout << "|" << res[i]  << "|" << std::endl;
    std::cout << "Cmd ------>" << command  << "|"<< std::endl;
    if (command == KICK)
        parseKick(res, fd);
    else if (command == "invite")
        parseInvite(res, fd);
    else if (command == MODE)
        parseMode(res, fd);
    else if (command == TOPIC)
        parseTopic(res, fd);
    else if (command == QUIT)
        parseQuit(fd);
    else if (command == PART)
        parsePart(res, fd);
    else if (command == NAMES)
        parseNames(res, fd);
    else if (command == LIST)
        parseList(res, fd);
    else if (command == PRIVMSG)
        parsePrivmsg(res, fd);
    else if(command == "nick")
        parseNick(res, fd);
    else if (command == JOIN)
        parseJoin(res, fd);
    else if (command == "/joke" || command == "/time")
        runBot(command, fd);
    else if (command == "notice")
        parseNotice(res, fd);
    else {
        std::string msg = "421 " + __users[fd].getNickname() + " " + command + " :Unknown command\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
}

std::vector<std::string>    Server::split(std::string &line, char c)
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
        send_msg(fd, message);
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
            if(it != __channels.end())
            {
                std::cout << "{{" <<  __channels.find(chn[i])->second.getChannelInvited().size()  << "}}"<< std::endl;
            }
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
                send_msg(fd, message);
                break ;
            }
            //if user already exist
            if (it->second.getChannelClientt(fd) == 1)
            {
                message = ":" + GetUserName(fd) + " 443 * :You are already in this channel\n";
                send_msg(fd, message);
                break ;
            }
            //check if it's private
            if (it->second.getChannelType() == 1)
            {
                if (it->second.getChannelInvited().empty() && vec.size() != 2)
                {
                    message = ":" + GetUserName(fd) + " 473 * Cannot join channel (+i)\n";
                    send_msg(fd, message);
                    break ;
                }
                inv = it->second.getInvited(fd);
                if (inv && vec.size() != 2)
                {

                    // std::cout << fd << "|" << inv->second.getNickname() << std::endl;
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                    message = ":" + GetUserName(fd) + " JOIN " + chn[i] + "\n";
                    send_msg(fd, message);
                    break ;
                }
                if (it->second.getChannelPass() == 1)
                {
                    if ((k < key.size() && it->second.getChannelPassword() != key[k++]) || key.size() == 0)
                    {
                        message = ":" + GetUserName(fd) + " 475 * Cannot join channel (+k)\n";
                        send_msg(fd, message);
                        break ;
                    }
                    else
                    {
                        __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                        message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                        send_msg(fd, message);
                        break ;
                    }
                }
                else
                {
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                    message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                    send_msg(fd, message);
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
                        send_msg(fd, message);
                        break ;
                    }
                    else
                    {
                        __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                        message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                        send_msg(fd, message);
                        break ;
                    }
                }
                else  
                {
                    if (it->second.getChannelClientt(fd) == 1)
                    {
                        message = ":" + GetUserName(fd) + " 443 * :You are already in this channel\n";
                        send_msg(fd, message);
                        break ;
                    }
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getNickname());
                    message = ":" + GetUserName(fd) + " 332 JOIN :" + __channels[chn[i]].getChannelTopic() + "\n";
                    send_msg(fd, message);
                    break ;
                }
            }
        }
    }
}

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
        send_msg(fd, message);
        return ;
    }
    //channels mode
    //no such channel
    it = __channels.find(vec[0]);
    if (it == __channels.end())
    {
        message = ":" + GetUserName(fd) + " 403 * :No such channel\n";
        send_msg(fd, message);
        return ;
    }
    //channel without mode
    if (vec.size() == 1)
    {
        message = ":" + GetUserName(fd) + " 324 * :Not enough parameters\n";
        send_msg(fd, message);
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
                        send_msg(fd, message);
                        return ;
                    }
                    it->second.setChannelType(1);
                    message = ":" + GetUserName(fd) + " 342 * :Channel is private now\n";
                    send_msg(fd, message);
                    return ;
                }
                if (vec[1][1] == KEY)
                {
                    if (i != -1)
                    {
                        message = ":" + GetUserName(fd) + " 401 * :No such nick/channel\n";
                        send_msg(fd, message);
                        return ;
                    }
                    if (vec.size() == 3)
                    {
                        it->second.setChannelPass(1);
                        it->second.setChannelPassword(vec[2]);
                        message = ":" + GetUserName(fd) + " 400 * :We have set the password\n";
                        send_msg(fd, message);
                        return ;
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        send_msg(fd, message);
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
                                send_msg(fd, message);
                                return ;
                            }
                            it->second.setChannelModerator(i);
                            message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is a moderator now\n";
                            send_msg(fd, message);
                            return ;
                        }
                        else
                        {
                            message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is not a member of channel\n";
                            send_msg(fd, message);
                            return ;
                        }
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        send_msg(fd, message);
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
                        send_msg(fd, message);
                        return ;
                    }
                    it->second.setChannelType(0);
                    message = ":" + GetUserName(fd) + " 400 * :" + "channel " + it->second.getChannelName() + " is public now\n";
                    send_msg(fd, message);
                    return ;
                }
                if (vec[1][1] == KEY && i != -1)
                {
                    if (vec.size() == 2)
                    {
                        it->second.setChannelPass(0);
                        it->second.setChannelPassword("");
                        message = ":" + GetUserName(fd) + " 400 * :" + "channel " + it->second.getChannelName() + " is without password now\n";
                        send_msg(fd, message);
                        return ;
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        send_msg(fd, message);
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
                                send_msg(fd, message);
                                return ;
                            }
                            v_cl = it->second.getChannelModerator();
                            if (IsModerator(it->second, GetUserName(fd)))
                            {
                                RemoveModerator(it->second, vec[2]);
                                message = ":" + GetUserName(fd) + " 400 * :" + "The user " + vec[2] + " is no longer a moderator now\n";
                                send_msg(fd, message);
                                return ;
                            }
                            else
                            {
                                message = ":" + GetUserName(fd) + " 482 * :" + "The user " + vec[2] + " is not a moderator of channel\n";
                                send_msg(fd, message);
                                return ;
                            }
                        }
                        else
                        {
                            message = ":" + GetUserName(fd) + " 441 * :" + "The user " + vec[2] + " is not a member of channel\n";
                            send_msg(fd, message);
                            return ;
                        }
                    }
                    else
                    {
                        message = ":" + GetUserName(fd) + " 696 * :Invalid mode params\n";
                        send_msg(fd, message);
                        return ;
                    }
                }
            }
            else
            {
                message = ":" + GetUserName(fd) + " 324 * :You have set a mode of channel\n";
                send_msg(fd, message);
                return ;
            }
        }
        else
        {
            message = ":" + GetUserName(fd) + " 482 * :" + "The user " + vec[2] + " is not a moderator of channel\n";
            send_msg(fd, message);
            return ;
        }
    }
}

void    Server::send_msg(int fd, std::string msg)
{
    send(fd, msg.c_str(), msg.size(), 0);
}

std::string            Server::GetUserName(int fd)
{
    std::map<int, Client>::iterator it;
    std::string                     message;
    it = __users.find(fd);
    if (it != __users.end())
    {
        return (it->second.getNickname());
    }
    message = ":" + GetUserName(fd) + " 401 * :No such nick/channel\n";
    send_msg(fd, message);
    return ("EROOR");
}

int                    Server::IsModerator(Channel &channel, std::string username)
{
    std::vector<int>::iterator                  it_vec;

    if (std::find(channel.getChannelModerator().begin(), channel.getChannelModerator().end(), GetUserId(username)) != channel.getChannelModerator().end())
        return (1);
    return (-1);
}

void                    Server::RemoveModerator(Channel &channel, std::string username)
{
    std::vector<int>::const_iterator it = std::find(channel.getChannelModerator().begin(), channel.getChannelModerator().end(), GetUserId(username));
    if (it != channel.getChannelModerator().end())
        channel.getChannelModerator().erase(it);
}

void    Server::parsePrivmsg(std::vector<std::string> &vec, int fd)
{
    std::string message = "";
    if (vec.size() == 0)
    {
        message = ":" + __users[fd].getNickname() + " 461 " + "PRIVMSG " + "Not enough parameters\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    if (vec.size() < 2)
    {
        message = ":" + __users[fd].getNickname() + " 412" + " PRIVMSG " + "No text to send\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    std::string msg = "";
    if (vec[1][0] != ':')
        msg += ":";
    for (size_t i = 1; i < vec.size(); ++i)
    {
        msg += vec[i];
        if (i != vec.size() - 1)
            msg += " ";
    }
    std::string targets = vec[0];
    std::vector<std::string> targetsVec;
    while (targets.find(',') != std::string::npos)
    {
        targetsVec.push_back(targets.substr(0, targets.find(',')));
        targets.erase(0, targets.find(',') + 1);
    }
    targetsVec.push_back(targets);
    for (size_t i = 0; i < targetsVec.size(); ++i)
    {
        if (targetsVec[i][0] == CHANNEL_PREFIX) {
            std::map<std::string, Channel>::iterator it = __channels.find(targetsVec[i]);// search for channel in the map
            if ( it == __channels.end() ) {
                message = ":" + __users[fd].getNickname() + " 403 " + "PRIVMSG " + "No such channel\n";
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
            Channel channel = it->second;
            if (isInChannel(channel, fd) == false) {
                message = ":" + __users[fd].getNickname() + " 404 " + "PRIVMSG " + "Cannot send to channel\n";
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
            std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
            for (; it2 != channel.getChannelClients().end(); ++it2) {
                if (it2->first == fd)
                    continue;
                message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
                send(it2->first, message.c_str(), message.size(), 0);
            }
        } else {
            int userId = GetUserId(targetsVec[i]);
            if (userId == -1){
                message = ":" + __users[fd].getNickname() + " 401 " + "PRIVMSG " + "No such nick\n";
                send(fd, message.c_str(), message.size(), 0);
            } else if (userId == fd){
                message = ":" + __users[fd].getNickname() + " 400 " + "PRIVMSG " + "Cannot send to yourself\n";
                send(fd, message.c_str(), message.size(), 0);
            } else {
                message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
                send(userId, message.c_str(), message.size(), 0);
            }
        }
    }
}

bool    Server::isInChannel(Channel &channel, int fd) const
{
    std::map<int, Client>::const_iterator it = channel.getChannelClients().begin();
   
    for (; it != channel.getChannelClients().end(); ++it)
    {
        if (it->first == fd)
            return (true);
    }
    return (false);
}


void    Server::parseNames(std::vector<std::string> &vec, int fd)
{
    std::string message;
    if (vec.size() == 0)
    {
        std::map<int, Client>::const_iterator it = __users.begin();
        message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + "Users\n";
        send(fd, message.c_str(), message.size(), 0);
        for (; it != __users.end(); ++it)
        {
            message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + it->second.getUsername() + "\n";
            send(fd, message.c_str(), message.size(), 0);
        }
        message = ":" + __users[fd].getNickname() + " 366 " + "NAMES " + "= " + "End of /NAMES list\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    if (vec.size() > MAXPARAMS)
    {
        message = ":" + __users[fd].getNickname() + " 461 " + "NAMES " + "Too many arguments\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    std::vector<std::string> targetsVec = split(vec[0], ',');
    for (size_t i = 0; i < targetsVec.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = __channels.find(targetsVec[i]);

        if (it == __channels.end()) {
            message = ":" + __users[fd].getNickname() + " 403 " + "NAMES " + "No such channel\n";
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
        Channel channel = it->second;
        if (isInChannel(channel, fd) == false)
        {
            message = ":" + __users[fd].getNickname() + " 404 " + "NAMES " + "Cannot send to channel\n";
            send(fd, message.c_str(), message.size(), 0);
            return ;
        }
        std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
        message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + channel.getChannelName() + "\n";
        send(fd, message.c_str(), message.size(), 0);
        for (; it2 != channel.getChannelClients().end(); ++it2)
        {
            message = ":" + __users[fd].getNickname() + " 353 " + "NAMES " + "= " + it2->second.getUsername() + "\n";
            send(fd, message.c_str(), message.size(), 0);
        }
    }
}


void    Server::parseList(std::vector<std::string> &vec, int fd)
{
    std::string message;
    if (vec.size() > MAXPARAMS)
    {
        message = ":" + __users[fd].getNickname() + " 461 " + "LIST " + "Too many arguments\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    if (vec.size() == 0)
    {
        std::map<std::string, Channel>::iterator it = __channels.begin();
        message = ":" + __users[fd].getNickname() + " 321 " + "LIST " + "Channel :Users Name\n";
        send(fd, message.c_str(), message.size(), 0);
        for (; it != __channels.end(); ++it)
        {
            if (it->second.getChannelType() == 1 && !isInChannel(it->second, fd))
                continue;
            message = ":" + __users[fd].getNickname() + " 322 " + it->second.getChannelName() + " " + std::to_string(it->second.getChannelClients().size()) + " :" + it->second.getChannelTopic() + "\n";
            send(fd, message.c_str(), message.size(), 0);
        }
        send(fd, "323 End of /LIST\n", 18, 0);
        return ;
    }
    
    std::vector<std::string> chns = split(vec[0], ',');

    for (size_t i = 0; i < chns.size(); ++i) {
        std::map<std::string, Channel>::iterator it = __channels.find(chns[i]);
        if (it == __channels.end()) {
            message = ":" + __users[fd].getNickname() + " 403 " + "LIST " + "No such channel\n";
            send(fd, message.c_str(), message.size(), 0);
            return ;
        }
        Channel channel = it->second;
        if (channel.getChannelType() == 1 && isInChannel(channel, fd) == false)
        {
            message = ":" + __users[fd].getNickname() + " 404 " + "LIST " + "Cannot send to channel\n";
            send(fd, message.c_str(), message.size(), 0);
            return ;
        }
        message = ":" + __users[fd].getNickname() + " 322 " + channel.getChannelName() + " " + std::to_string(channel.getChannelClients().size()) + " :" + channel.getChannelTopic() + "\n";
        send(fd, message.c_str(), message.size(), 0);
    }
}

void    Server::parseKick(std::vector<std::string> &vec, int fd)
{
    std::string message;
    if (vec.size() == 0) {
        message = ":" + __users[fd].getNickname() + " 461 " + "KICK " + "Not enough parameters\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    } else if (vec.size() > 3){
        message = ":" + __users[fd].getNickname() + " 461 " + "KICK " + "Too many parameters\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    std::map<std::string, Channel>::iterator it = __channels.find(vec[0]);
    if (it == __channels.end()) {
        message = ":" + __users[fd].getNickname() + " 403 " + "KICK " + "No such channel\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    Channel channel = it->second;
    if (!isInChannel(channel, fd))
    {
        message = ":" + __users[fd].getNickname() + " 442 " + "KICK " + "You're not on that channel\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    std::vector<int>::iterator it3 = std::find(channel.getChannelModerator().begin(), channel.getChannelModerator().end(), fd);
    if (it3 == channel.getChannelModerator().end())
    {
        message = ":" + __users[fd].getNickname() + " 482 " + "KICK " + "You're not a channel operator\n";
        send(fd, message.c_str(), message.size(), 0);
        return ;
    }
    std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
    for (; it2 != channel.getChannelClients().end(); ++it2)
    {
        std::cout << "{" << it2->second.getNickname() << "}" << std::endl;
        if (it2->second.getNickname() == vec[1] && it2->first != fd)
        {
            message = ":" + __users[fd].getNickname() + " KICK " + channel.getChannelName() + " " + vec[1] + " :" + "Kicked by " + __users[fd].getNickname() + "\n";
            send(it2->first, message.c_str(), message.size(), 0);
            channel.eraseClient(it2->first);
            return ;
        }
    }
    std::cout << "|" << vec[1] << "|" << std::endl;
    message = ":" + __users[fd].getNickname() + " 441 " + "KICK " + "They aren't on that channel\n";
    send(fd, message.c_str(), message.size(), 0);
}

void    insertJokes(std::vector<std::string>& jokes)
{
    jokes.push_back("What do you call a fake noodle? An impasta.");
    jokes.push_back("How many apples grow on a tree? All of them.");
    jokes.push_back("Want to hear a joke about paper? Nevermind it's tearable.");
    jokes.push_back("I just watched a program about beavers. It was the best dam program I've ever seen.");
    jokes.push_back("Why did the coffee file a police report? It got mugged.");
    jokes.push_back("How does a penguin build it's house? Igloos it together.");
    jokes.push_back("Dad, did you get a haircut? No I got them all cut.");
    jokes.push_back("This graveyard looks overcrowded. People must be dying to get in there.");
    jokes.push_back("Did you hear about the actor who fell through the floorboards? He was just going through a stage.");
    jokes.push_back("Did you hear about the claustrophobic astronaut? He just needed a little space.");
    jokes.push_back("Why do you never see elephants hiding in trees? Because they're so good at it.");
    jokes.push_back("Dad, can you put my shoes on? No, I don't think they'll fit me.");
    jokes.push_back("What do you call a belt made out of watches? A waist of time.");
    jokes.push_back("Why did the scarecrow win an award? Because he was outstanding in his field.");
    jokes.push_back("Why don't skeletons ever go trick or treating? Because they have no body to go with.");
    jokes.push_back("What do you call a dog that does magic tricks? A labracadabrador.");
}

void    Server::runBot(const std::string& command, int fd)
{
    std::vector<std::string> jokes;

    insertJokes(jokes);

    std::string message;

    if (command == "/joke")
    {
        std::string  res = "";
        int status = system("which curl > /dev/null");
        if (status != 0){
            res = jokes[rand() % jokes.size()];
        } else {
            status = system("curl -HI --silent  --output /dev/null  \"Accept: text/plain\" https://icanhazdadjoke.com/ > /tmp/.joke");
            if (status != 0) {
                res = jokes[rand() % jokes.size()];
            } else {
                int jokeFd = open("/tmp/.joke", O_RDONLY);
                char buf;
                int rbytes;
                while ((rbytes = read(jokeFd, &buf, 1) > 0))
                    res += buf;
                res += "\n";
                close(jokeFd);
            }
        }
        message = ":" + __users[fd].getNickname() + " PRIVMSG " + __users[fd].getNickname() + " :" + res + "\n";
    } else if (command  == "/time") {
        std::time_t now = std::time(NULL);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        std::string current_time(buffer);
        current_time += "\n";
        message = ":" + __users[fd].getNickname() + " PRIVMSG " + __users[fd].getNickname() + " :" + current_time + "\n";
    }
    
    send(fd, message.c_str(), message.size(), 0);
}

std::string Server::getHostname() const
{
    return __hostname;
}

void    Server::parseNotice(std::vector<std::string> &vec, int fd)
{
    std::string message = "";
    if (vec.size() == 0)
        return ;
    if (vec.size() < 2)
        return ;
    std::string msg = "";
    if (vec[1][0] != ':')
        msg += ":";
    for (size_t i = 1; i < vec.size(); ++i)
    {
        msg += vec[i];
        if (i != vec.size() - 1)
            msg += " ";
    }
    std::string targets = vec[0];
    std::vector<std::string> targetsVec;
    while (targets.find(',') != std::string::npos)
    {
        targetsVec.push_back(targets.substr(0, targets.find(',')));
        targets.erase(0, targets.find(',') + 1);
    }
    targetsVec.push_back(targets);
    for (size_t i = 0; i < targetsVec.size(); ++i)
    {
        if (targetsVec[i][0] == CHANNEL_PREFIX) {

            std::map<std::string, Channel>::iterator it = __channels.find(targetsVec[i]);// search for channel in the map

            if ( it == __channels.end() ) {
                return;
            }
            Channel channel = it->second;
            if (isInChannel(channel, fd) == false) {
                return;
            }
            std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
            for (; it2 != channel.getChannelClients().end(); ++it2) {
                if (it2->first == fd)
                    continue;
                message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
                send(it2->first, message.c_str(), message.size(), 0);
            }
        } else {
            int userId = GetUserId(targetsVec[i]);
            if (userId != -1 && userId != fd){
                message = ":" + __users[fd].getNickname() + " PRIVMSG " + targetsVec[i] + " " + msg + "\n";
                send(userId, message.c_str(), message.size(), 0);
            }
        }
    }
}