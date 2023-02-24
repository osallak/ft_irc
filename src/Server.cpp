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
    if(cmd.first != "Error")
    {
        if(__NewConnections.find(UserId)->second.getPassword().empty())
        {
            if(cmd.first == "pass")
            {
                if(cmd.second != __password)
                    std::cout << "ERR_PASSWDMISMATCH : 464\n";
                else 
                    __NewConnections.find(UserId)->second.setPassword(cmd.second);
            }
            else
                std::cout << "u should give me Password first\n";
        }
        else if(__NewConnections.find(UserId)->second.getNickname().empty())
        {
            
            if(cmd.first == "nick")
            {
                if(__users.find(GetUserId(cmd.second)) != __users.end())
                    std::cout << " ERR_NICKNAMEINUSE (433)  \n";
                else
                    __NewConnections.find(UserId)->second.setNickname(cmd.second);
            }
            else
                std::cout << "u should give me Nickname first\n";
        }
        else if(__NewConnections.find(UserId)->second.getUsername().empty())
        {
            if(cmd.first == "user")
            {
                if(cmd.second.empty())
                    std::cout << "ERR_NEEDMOREPARAMS (461)\n";
                else if(__users.find(GetUserId(cmd.second)) != __users.end())
                    std::cout << " ERR_ALREADYREGISTERED (462) \n";
                else
                {
                    __NewConnections.find(UserId)->second.setUsername(cmd.second);
                    std::cout << __NewConnections.find(UserId)->second.getUsername() << " has been connected successfully\n";
                    __NewConnections.find(UserId)->second.setIsLogged(true);
                    __users[UserId] = __NewConnections.find(UserId)->second;
                }
            }
        }
    }
}
std::vector<std::pair<std::string, std::string> > Server::ParceConnectionLine(std::string cmd)
{
    cmd = trim(cmd);
    std::vector<std::pair<std::string,std::string> >ret;
    std::size_t __found  = cmd.find("\n");
    std::vector<std::string>ConnectionInf(3);
    ConnectionInf[0] =  cmd.substr(0,__found);
    cmd = cmd.substr(__found + 1 ,- 1);
    __found =  cmd.find("\n");
    ConnectionInf[1] = cmd.substr(0,__found);
    ConnectionInf[2] = cmd.substr(__found + 1 , -1);
    for(unsigned int i = 0 ; i < 3;i++)
    {
        ConnectionInf[i] = trim( ConnectionInf[i]);
    }
    for(int i = 0 ; i < 3;i++)
    {
        __found = ConnectionInf[i].find(" ");
        if(__found == std::string::npos)
            return(ret);
        ret.push_back(std::make_pair(trim(ConnectionInf[i].substr(0,__found)),trim(ConnectionInf[i].substr(__found + 1, -1))));
    }
    if((ret[0].first != "password" || ret[0].second != __password ) || ret[2].first != "nickname" || ret[1].first != "username")
    {
        ret.push_back(std::make_pair("error","error"));
    }
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
        ret.second = cmd.substr(found + 1, cmd.size() - found - 2);
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

int    Server::GetUserId(std::string UserName)
{
    std::map<int, Client>::iterator it;
    for (it = __users.begin(); it != __users.end(); ++it) {
        if(it->second.getUsername() == UserName)
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
    __users.erase(__UserId);
    __NewConnections.erase(__UserId);
     std::cout << "Client disconnected" << std::endl;
     close(__pollfds[__currentNdx].fd);
     __pollfds.erase(__pollfds.begin() + __currentNdx);
    //  close(__UserId);
    // __pollfds.erase(__pollfds.begin() + i);
}
void Server::parseQuit(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    DeleteUser(__UserId);
    std::string msg = "Quit:";
    for(size_t i = 0; i < __arg.size();i++)
    {
        if(i != 0)
            msg+=" ";
        msg+=__arg[i];
    }
    std::map<int, Client>::iterator it = __users.begin();
    msg+="\n";
    while(it != __users.end())
    {
        __ValRead = send(it->first,msg.c_str(),msg.size(), 0);
        it++;
    }
}
void Server::parsePart(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    if(!__arg.size())
        __ValRead = send(__UserId,"ERR_NEEDMOREPARAMS (461)\n",26, 0);
    std::vector<std::string>channels = split(__arg[0],',');
    for(size_t i = 0; i < channels.size();i++)
    {
        if(__channels.find(channels[i]) == __channels.end())
            __ValRead = send(__UserId,"ERR_NOSUCHCHANNEL (403)\n",25, 0);
        else if(__channels.find(channels[i])->second.getChannelClients().find(__UserId) ==
             __channels.find(channels[i])->second.getChannelClients().end())
            __ValRead = send(__UserId,"ERR_NOTONCHANNEL (442)\n",26, 0);
        else
            __channels[channels[i]].eraseClient(__UserId);
        if(__ValRead == -1)
            std::cout << "send() failde\n";
        __ValRead = 0;
    }
}
void Server::parseInvite(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    if(!__arg.size())
    {
        __ListChannelsUserInvTo(__UserId);
        return;
    }
    int __ReceiverId = GetUserId(__arg[0]);
      std::vector<int> vec = __channels.find(__arg[1])->second.getChannelModerator();
    if(__arg.size() == 1)
        __ValRead = send(__UserId,"ERR_NOSUCHCHANNEL  (482)\n",26, 0);
    else if(__ReceiverId == -1)
        __ValRead = send(__UserId,"ERR_NOSUCHNICK (401)\n",22, 0);
    else if(!__channels[__arg[1]].getClientNb())
        __ValRead = send(__UserId,"ERR_NOSUCHCHANNEL  (482)\n",26, 0);
    else if(__channels[__arg[1]].getChannelClient(__users[__UserId].getUsername()) == -1)
        __ValRead = send(__UserId,"ERR_NOSUCHCHANNEL  (482)\n",26, 0);
    else if(__channels.find(__arg[1])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) != vec.end())
        __ValRead = send(__UserId,"ERR_CHANOPRIVSNEEDED (482)\n",28, 0);
    else
    {
        __ValRead = send(__UserId,"RPL_INVITING (341)\n",20, 0);
        __channels[__arg[1]].SetInviteds(__ReceiverId, __users[__UserId]);
    }
    if(__ValRead == - 1)
        std::cout << "send() failde\n";

}

void Server::parseTopic(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    std::cout << "im here\n";
    if(!__arg.size() || __arg.size() == 1)
    {
            __ValRead = send(__UserId,"RPL_NOTOPIC (331)\n",19, 0);
            return;
    }
    std::vector<int> vec = __channels.find(__arg[0])->second.getChannelModerator();
    if(__channels.find(__arg[0]) == __channels.end())
        __ValRead = send(__UserId,"ERR_NOSUCHCHANNEL (403)\n",25, 0);
    else if(__channels.find(__arg[0])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) != vec.end())
        __ValRead = send(__UserId,"ERR_CHANOPRIVSNEEDED (482)\n",28, 0);

    else
    {
        std::map<int, Client>::const_iterator BeginIt;
        std::map<int, Client>::const_iterator EndIt;
        BeginIt = __channels.find(__arg[0])->second.BigenIterator();
        EndIt = __channels.find(__arg[0])->second.EndIterator();
        while(BeginIt != EndIt)
        {
             __channels.find(__arg[0])->second.setChannelTopic(__arg[1]);
            size_t len = __channels.find(__arg[0])->second.getChannelTopic().size();
            __ValRead = send(BeginIt->first,__channels.find(__arg[0])->second.getChannelTopic().c_str(),len, 0);
            __ValRead = send(BeginIt->first,"\n",1, 0);
            BeginIt++;
        }
    }
    if(__ValRead == - 1)
        std::cout << "send() failde\n";
}
// int    Server::GetUserId(std::string UserName)
// {
//     std::map<int, Client>::iterator it;
//     for (it = __users.begin(); it != __users.end(); ++it) {
//         if(it->second.getUsername() == UserName)
//             return(it->first);
//     }

//     return -1;
// }
// void    Server::__ListChannelsUserInvTo(int UserId)
// {

//     if(send(UserId,"channels can you access\n",25,0) == -1)
//     {
//         std::cout << "send() Failed\n";
//                 exit(0);
//     }
//     std::map<std::string, Channel>::iterator it;
//     for (it = __channels.begin(); it != __channels.end(); ++it) {
//         if(it->second.getInvited(UserId))
//         {
//             if (send(UserId,it->second.getChannelName().c_str(),it->second.getChannelName().size(), 0) == - 1)
//             {
//                 std::cout << "send() Failed\n";
//                 exit(0);
//             }
//         }
//     }
// }

// void Server::parseInvite(std::vector<std::string>__arg,int __UserId)
// {
//     int __ValRead = 0;
//     if(!__arg.size())
//     {
//         __ListChannelsUserInvTo(__UserId);
//         return;
//     }
//     int __ReceiverId = GetUserId(__arg[0]);
//     if(__arg.size() == 1)
//         __ValRead = send(__UserId,"341\n",5, 0);
//     else if(__ReceiverId == -1)
//         __ValRead = send(__UserId,"403\n",5, 0);
//     else if(__channels.find(__arg[1]) == __channels.end())
//         __ValRead = send(__UserId,"403",4, 0);
//     else if(!__channels[__arg[1]].getClientNb())
//         __ValRead = send(__UserId,"482\n",5, 0);
//     else if(!__channels[__arg[1]].getInvited(__UserId))
//         __ValRead = send(__UserId,"442\n",5, 0);
//     else
//     {
//         __ValRead = send(__UserId,"341\n",5, 0);
//         __channels[__arg[1]].SetInviteds(__ReceiverId, __users[__UserId]);
//         // __channels[__arg[1]].setChannelClients(__ReceiverId , __arg[0]);
//     }
//     if(__ValRead == - 1)
//         std::cout << "send() failde\n";

// }

// void Server::parseTopic(std::vector<std::string>__arg,int __UserId)
// {
//     int __ValRead = 0;
//     std::vector<int> vec = __channels.find(__arg[1])->second.getChannelModerator();
//     if(!__arg.size())
//         std::cout << "NO arg\n";
//     else if(__arg.size() == 1)
//         __ValRead  = send(__UserId,"331\n",5, 0);
//     else if(__channels.find(__arg[1]) == __channels.end())
//         __ValRead = send(__UserId,"403",4, 0);
//     else if(__channels.find(__arg[1])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) != vec.end())
//         __ValRead = send(__UserId,"482",4, 0);
//     else
//     {
//         std::map<int, Client>::const_iterator BeginIt;
//         std::map<int, Client>::const_iterator EndIt;
//         BeginIt = __channels.find(__arg[1])->second.BigenIterator();
//         EndIt = __channels.find(__arg[1])->second.EndIterator();
//         while(BeginIt != EndIt)
//         {
//             size_t len = __channels.find(__arg[1])->second.getChannelTopic().size();
//              __channels.find(__arg[1])->second.setChannelTopic(__arg[2]);
//             __ValRead = send(BeginIt->first,__channels.find(__arg[1])->second.getChannelTopic().c_str(),len, 0);
//             __ValRead = send(BeginIt->first,"\n",1, 0);
//             BeginIt++;
//         }
//     }
//     if(__ValRead == - 1)
//         std::cout << "send() failde\n";
// }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

        
    // valread = send(__pollfds[i].fd, msg.append("\n").c_str(),msg.size(), 0);
    //  if (valread < 0)
    // {
    //     std::cerr << "Error: send failed\n";
    //     return(false);
    // }
// }

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

// std::string Server::getCommand() const
// {
//     return (this->__users.);
// }

// void Server::setCommand(std::string password)
// {
//     if (password.empty())
//     {
//         std::cerr << "Password cannot be empty" << std::endl;
//         exit(1);
//     }
//     this->__password = password;
// }

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
    if (fcntl(__socket, F_SETFD, O_NONBLOCK)  < 0)// check if this is the right way to do it
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
    // __spollfd.revents = 0;
    __pollfds.push_back(__spollfd);// add the server socket to the pollfds vector, to keep track of it
    
    // int timeout = (1000 * 60 ); // 1 minute
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
        if (ret == 0)
        {
            std::cerr << "Error: poll timeout" << std::endl;
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
                struct pollfd __NewClient;
                __NewClient.fd = new_socket;
                __NewClient.events = POLLIN;
                __pollfds.push_back(__NewClient);
                Client NewClient = Client();
                __NewConnections[__NewClient.fd] = NewClient;
                 // add the new client socket to the pollfds vector
                
            }
            else // if the event is on a client socket, it means the client sent a message
            {
                // TODO: check if the client is trying to log in or send a command, fugure out how to add it to clients vector
                int valread;
                char buffer[1024] = {0};
                if ((valread = read(__pollfds[i].fd, buffer, 1024)) < 0)
                {
                    std::cerr << "Error: read failed" << std::endl;
                    return (false);
                }
                if (valread == 0) // if the client disconnected
                {
                    std::cout << "Client disconnected" << std::endl;
                    close(__pollfds[i].fd);
                    __pollfds.erase(__pollfds.begin() + i); // remove the client socket from the pollfds vector
                                                            //TODO: remove the client from the clients vector
                }
                else // this is just for testing, it should be parsed and executed
                {
                    std::string tmpBuffer = buffer;
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
                                SetUserInf(ParceConnection(CurrentBuffer), __pollfds[i].fd);
                            else
                            {
                                std::vector<std::pair<std::string, std::string> > cmds = ParceConnectionLine(CurrentBuffer);
                                if(cmds.size() != 3)
                                    std::cout << "correct Form : Pass <password> \\n nick <nickname> \\n user <username>\n";
                                else
                                {
                                    __NewConnections.find(__pollfds[i].fd)->second.setPassword(cmds[0].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setUsername(cmds[1].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setUsername(cmds[2].second);
                                }

                            }
                            __NewConnections.find(__pollfds[i].fd)->second.setBuffer("");
                        }
                        // this means the client is not authenticated yet
                    }
                    // std::cout << "Client: \t";
                    // std::cout << buffer << std::endl;
                }
                // std::string msg;std::cout << "enter message ";std::getline(std::cin, msg);
                // // std::cout << msg << std::endl;
                // valread = send(__pollfds[i].fd, msg.append("\n").c_str(),msg.size(), 0);
                //  if (valread < 0)
                // {
                //     std::cerr << "Error: send failed\n";
                //     return(false);
                // }
                // to be continued...
            }
        }
    }
    return (true);
}

void Server::disconnect( void )
{
    //disconnect from the server
    // it should be called with the client to disconnect as a parameter (or something like that)
}

int Server::authentification( void )
{
    //authentification with the server, return 0 if success, 1 if not
    // it receives the password from the client and compares it to the one in the server
    // if it's the same, it returns 1, else it returns 0
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
    // line = "KICK myChan ayoub anjaimi";
    while ((pos = line.find(' ')) != std::string::npos)
    {
        str = line.substr(0, pos);
        res.push_back(str);
        line.erase(0, pos + 1);

    }
    line = line.substr(0,line.size() - 1);
    res.push_back(line);
    // res[res.size() - 1] =  res[res.size() - 1].substr(0, res[res.size() - 1].size() - 1);
    command = res[0];
    for (size_t i = 0; i < command.size(); ++i){
        command[i] = (char)(tolower(command[i]));
    }
    // for(size_t i = 0 ; i < res.size();i++)
    //     std::cout << res[i] << std::endl;
    // (void)fd;
        // if (command == KICK)
        //     parseKick(res, fd);
    // else if (command == MODE)
        // parseMode(res, fd);
    res.erase(res.begin());
    for(size_t i = 0 ; i < res.size();i++)
        std::cout << res[i] << std::endl;
    // (void)fd;
    std::cout << "Cmd ------>" << command  << "|"<< std::endl;
    std::cout << res.size() << std::endl;
    // if (command == KICK)
    //     parseKick(res, fd);
     if (command == "invite")
        parseInvite(res, fd);
    else if (command == MODE)
        parseMode(res, fd);
    else if (command == TOPIC)
        parseTopic(res, fd);
    // else if (command == PING)
    //     parsePing(res, fd);
    // else if (command == PONG)
    //     parsePong(res, fd);
    else if (command == QUIT)
        parseQuit(res, fd);
    else if (command == PART)
        parsePart(res, fd);
    else if (command == NAMES)
    {
        // std::cout  << "if (command == NAMES)\n";
        parseNames(res, fd);
    }
    else if (command == LIST)
        parseList(res, fd);
    else if (command == PRIVMSG)
        parsePrivmsg(res, fd);
    else if (command == JOIN)
        parseJoin(res, fd);
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
    std::map<int,Client>::iterator              inv;
    std::string                                 str;
    size_t                                      i;
    // size_t                                      j;
    size_t                                      k;

    k = 0;
    if (vec.size() == 0)
    {
        std::cout << "ERR_NEEDMOREPARAMS(461)\n";
        return ;
    }
    // std::cout << "|" << vec[1] << "|" << std::endl;
    if (vec[0][0] != '#' || !vec[0][1])
    {
        std::cout << "Bad channel name\n";
        return ;
    }
    chn = split(vec[0], ',');
    std::cout << "size : " << chn.size() << std::endl;
    if (vec.size() == 2)
        key = split(vec[1], ',');
    //check if channels exist
    for (i = 0;i < chn.size(); ++i)
    {
        while (1)
        {
            puts("here1");
            it = __channels.find(chn[i]);
            if (it == __channels.end())
            {
                __channels[chn[i]].setChannelName(chn[i]);
                __channels[chn[i]].setChannelTopic("");
                __channels[chn[i]].setChannelClients(fd, __users[fd].getUsername());
                __channels[chn[i]].setChannelModerator(fd);
                __channels[chn[i]].setChannelType(0);
                __channels[chn[i]].setChannelPass(0);
                __channels[chn[i]].setChannelPassword("");
                if (vec.size() == 2 && key.size() > i)
                {
                    __channels[chn[i]].setChannelPass(1);
                    __channels[chn[i]].setChannelPassword(key[i]);
                }
                std::cout << "Channel " << chn[i] << " created succesfully\n";
                break ;
            }
            //if user already exist
            if (it->second.getChannelClientt(fd) == 1)
            {
                std::cout << "User already exist in the channel " << chn[i] << "\n";
                break ;
            }
            //check if it's private
            if (it->second.getChannelType() == 1)
            {
                if (it->second.getChannelInvited().empty())
                {
                    std::cout << "ERR_INVITEONLYCHAN(473)\n";
                    break ;
                }
                inv = it->second.getChannelInvited().find(fd);
                if (inv == it->second.getChannelInvited().end())
                {
                    std::cout << "ERR_INVITEONLYCHAN(473)\n";
                    break ;
                }
                std::cout << inv->second.getUsername() << std::endl;
                break ;
                if (it->second.getChannelPass() == 1)
                {
                    if (it->second.getChannelPassword() != key[k++])
                    {
                        std::cout << "ERR_BADCHANNELKEY(475)\n";
                        break ;
                    }
                    else
                    {
                        __channels[chn[i]].setChannelClients(fd, __users[fd].getUsername());
                        std::cout << "Joined to the channel "<< chn[i] << "\n";
                        break ;
                    }
                }
            }
            else
            {
                if (it->second.getChannelPass() == 1)
                {
                    if (key.size() <= i || it->second.getChannelPassword() != key[k++])
                    {
                        std::cout << "ERR_BADCHANNELKEY(475)\n";
                        break ;
                    }
                    else
                    {
                        __channels[chn[i]].setChannelClients(fd, __users[fd].getUsername());
                        std::cout << "Joined to the channel "<< chn[i] << "\n";
                        break ;
                    }
                }
                else  
                {
                    if (it->second.getChannelClientt(fd) == 1)
                    {
                        std::cout << "User already exist in the channel " << chn[i] << "\n";
                        break ;
                    }
                    __channels[chn[i]].setChannelClients(fd, __users[fd].getUsername());
                    std::cout << "Joined to the channel "<< chn[i] << "\n";
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
    std::vector<int>                         v_cl;
    int                                      i;

    if (vec.size() == 0)
    {
        std::cout << "ERR_NEEDMOREPARAMS(461)\n";
        return ;
    }
    (void)fd;
    //channels mode
    //no such channel
    it = __channels.find(vec[0]);
    if (it == __channels.end())
    {
        std::cout << "ERR_NOSUCHCHANNEL(403)\n";
        return ;
    }
    //channel without mode
    if (vec.size() == 1)
    {
        std::cout << "RPL_CHANNELMODEIS(324)\n";
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
            if (vec[1][0] == '+')
            {
                if (vec[1][1] == 'i')
                {
                    if (it->second.getChannelType() == 1)
                    {
                        std::cout << "channel " << it->second.getChannelName() << " is already private\n";
                        return ;
                    }
                    it->second.setChannelType(1);
                    std::cout << "channel " << it->second.getChannelName() << " is private now\n";
                    return ;
                }
                if (vec[1][1] == 'k')
                {
                    if (i != -1)
                    {
                        std::cout << "error\n";
                        return ;
                    }
                    if (vec.size() == 3)
                    {
                        it->second.setChannelPass(1);
                        it->second.setChannelPassword(vec[2]);
                        std::cout << "We have set the password\n";
                        return ;
                    }
                    else
                    {
                        std::cout << "ERR_INVALIDMODEPARAM(696)\n";
                        return ;
                    }
                }
                if (vec[1][1] == 'm')
                {
                    if (vec.size() == 3)
                    {
                        if (i != -1)
                        {
                            it->second.setChannelModerator(i);
                            std::cout << "User with fd " << i << " is a moderator now\n";
                            return ;
                        }
                        else
                        {
                            std::cout << "User with fd " << i << " is not a member of channel\n";
                            return ;
                        }
                    }
                }
                // if (vec[1][1] == '')
            }
            else if (vec[1][0] == '-')
            {
                if (vec[1][1] == 'i')
                {
                    if (it->second.getChannelType() == 0)
                    {
                        std::cout << "channel " << it->second.getChannelName() << " is already public\n";
                        return ;
                    }
                    it->second.setChannelType(0);
                    std::cout << "channel " << it->second.getChannelName() << " is public now\n";
                    return ;
                }
                if (vec[1][1] == 'k' && i != -1)
                {
                    if (vec.size() == 2)
                    {
                        it->second.setChannelPass(0);
                        it->second.setChannelPassword("");
                        std::cout << "The channel " << it->second.getChannelName() << " is without password now\n";
                        return ;
                    }
                    else
                    {
                        std::cout << "ERR_INVALIDMODEPARAM(696)\n";
                        return ;
                    }
                }
                if (vec[1][1] == 'm')
                {
                    if (vec.size() == 3)
                    {
                        i = it->second.getChannelClient(vec[2]);
                        if (i != -1)
                        {
                            v_cl = it->second.getChannelModerator();
                            std::vector<int>::const_iterator it = std::find(v_cl.begin(), v_cl.end(), fd);
                            if (it != v_cl.end())
                            {
                                v_cl.erase(it);
                                std::cout << "User with fd " << fd << " is no longer a moderator now\n";
                                return ;
                            }
                            else
                            {
                                std::cout << "User with fd " << i << " is not a moderator of channel\n";
                                return ;
                            }
                        }
                        else
                        {
                            std::cout << "User with fd " << i << " is not a member of channel\n";
                            return ;
                        }
                    }
                }
            }
            else
            {
                std::cout << "RPL_CHANNELMODEIS(324)\n";
                return ;
            }
        }
        else
        {
            std::cout << "ERR_CHANOPRIVSNEEDED(482)\n";
            return ;
        }
    }
    // //     parseList(res, fd);
    // if (command == PRIVMSG)
    //     parsePrivmsg(res, fd);
    // else if (command == JOIN)
    //     parseJoin(res, fd);
}

// void    Server::parseKick(std::vector<std::string> &vec, int fd)
// {
//     size_t i;

//     (void)fd;
//     if (vec.size() <= 1)
//         std::cout << "ERR_NEEDMOREPARAMS(461)\n";
//     for (i = 0;i < __channels.size();++i)
//     {
//         if (__channels[vec[0]].getChannelName() == vec[0])
//             break ;
//     }
//     if (i == __channels.size())
//         std::cout << "ERR_NOSUCHCHANNEL(403\n)";
//     // if (__users[fd]. == '')
// }

// std::vector<std::string>    Server::split(std::string &line, char c)
// {
//     std::vector<std::string>    vec;
//     size_t                      pos = 0;
//     std::string                 str;

//     while ((pos = line.find(c)) != std::string::npos)
//     {
//         str = line.substr(0, pos);
//         vec.push_back(str);
//         line.erase(0, pos + 1);
//     }
//     vec.push_back(line);
//     return (vec);
// }

// void    Server::parseJoin(std::vector<std::string> &vec, int fd)
// {
//     std::vector<std::string>                    chn;
//     std::vector<std::string>                    key;
//     std::map<std::string,Channel>::iterator     it;
//     std::map<int,Client>::iterator              inv;
//     std::string                                 str;
//     size_t                                      i;
//     size_t                                      k;

//     k = 0;
//     if (vec.size() == 0)
//     {
//     // size_t i;
//     std::cout << "hello\n";
//     (void)fd;
//     if (vec.size() <= 1)
//         std::cout << "ERR_NEEDMOREPARAMS(461)\n";
//         return ;
//     }
//     chn = split(vec[0], ',');
//     key = split(vec[1], ',');
//     //check if channels exist
//     for (i = 0;i < chn.size(); ++i)
//     {
//         it = __channels.find(chn[i]);
//         if (it == __channels.end())
//         {
//             __channels[chn[i]].setChannelName(chn[i]);
//             __channels[chn[i]].setChannelTopic("");
//             __channels[chn[i]].setChannelClients(fd, __users[fd].getUsername());
//             __channels[chn[i]].setChannelModerator(fd);
//             __channels[chn[i]].setChannelType(0);
//             __channels[chn[i]].setChannelPass(0);
//             __channels[chn[i]].setChannelPassword("");
//             std::cout << "Channel creted succesfully\n";
//             return ;
//         }
//         //check if it's private
//         if (it->second.getChannelType() == 1)
//         {
//             inv = it->second.getChannelInvited().find(fd);
//             if (inv == it->second.getChannelInvited().end())
//             {
//                 std::cout << "ERR_INVITEONLYCHAN(473)\n";
//                 return ;
//             }
//             if (it->second.getChannelPass() == 1)
//             {
//                 if (it->second.getChannelPassword() != key[k++])
//                 {
//                     std::cout << "ERR_BADCHANNELKEY(475)\n";
//                     return ;
//                 }
//                 else
//                 {
//                     std::cout << "Joined\n";
//                     return ;
//                 }
//             }
//         }
//         else
//         {
//             if (it->second.getChannelPass() == 1)
//             {
//                 if (it->second.getChannelPassword() != key[k++])
//                 {
//                     std::cout << "ERR_BADCHANNELKEY(475)\n";
//                     return ;
//                 }
//                 else
//                 {
//                     std::cout << "Joined\n";
//                     return ;
//                 }
//             } else  {
//                 std::cout << "Joined\n";
//                 return ;
//             }
//         }
//     }
// }

// void    Server::parseMode(std::vector<std::string> &vec, int fd)
// {
//     std::map<int,Client>::iterator  it;
//     // size_t                          i;

//     if (vec.size() == 0)
//     {
//         std::cout << "ERR_NEEDMOREPARAMS(461)\n";
//         return ;
//     }
//     (void)fd;
//     // it = __users.find()
//     // for (i = 0;i < __channels.size();++i)
//     // {
//     //     if (__channels[vec[0]].getChannelName() == vec[0])
//     //         break ;
//     // }
//     // if (i == __channels.size())
//     //     std::cout << "ERR_NOSUCHCHANNEL(403)\n";
//     // it = std::find(__channels[vec[0]].getChannelModerator().begin(), __channels[vec[0]].getChannelModerator().end(), fd);
//     // if (it == __channels[vec[0]].getChannelModerator().end())
//     //     std::cout << "ERR_CHANOPRIVSNEEDED(482)\n";
// }
void    Server::parsePrivmsg(std::vector<std::string> &vec, int fd)
{
    //consider that the command is already erased from the vector
    if (vec.size() == 0)
    {
        std::cout << "ERR_NEEDMOREPARAMS(461)\n"; 
        return ;
    }
    if (vec.size() > MAXPARAMS)
    {
        std::cout << "ERR_TOOMANYARGUMENTS(461)\n";// TODO: check if it's the right error code
        return ;
    }
    
    std::string msg = "";

    std::cout << "privmsg: "<<vec[1] << std::endl;
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
                std::cout << "ERR_NOSUCHCHANNEL(403)\n";
                return;
            }
            // TODO: check if the user is in the channel
            Channel channel = it->second;
            if (isInChannel(channel, fd) == false)
            {
                std::cout << "ERR_CANNOTSENDTOCHAN(404)\n";
                return;//TODO: continue or return?
            }
            std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
            for (; it2 != channel.getChannelClients().end(); ++it2)
            {
                std::cout << "MSG TO USER: " << it2->second.getUsername() << " " << msg << std::endl;
            }
        } else {
            int userId = GetUserId(targetsVec[i]);//TODO: implement getUserId
            if (userId == -1)
                std::cout << "ERR_NOSUCHNICK(401)\n";
            else {
                std::cout << "MSG TO USER: " << targetsVec[i] << " :" << msg << std::endl;
                //TODO: send message to the user

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

void    listAllUsers(std::map<int,Client> const &__users)
{
    std::map<int, Client>::const_iterator it = __users.begin();

    std::cout << "USERS: " << std::endl;
    for (; it != __users.end(); ++it)
    {
       std::cout << "\t" << it->second.getUsername() << std::endl; 
    }
}

void    Server::parseNames(std::vector<std::string> &vec, int fd)
{
    // std::cout << "NAMES\n" << std::endl;
    if (vec.size() == 0)
    {
        listAllUsers(__users);
        return ;
    }
    if (vec.size() > MAXPARAMS)
    {
        std::cout << "ERR_TOOMANYARGUMENTS(461)\n";// TODO: check if it's the right error code
        return ;
    }
    for (size_t i = 0; i < vec.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = __channels.find(vec[i]);// search for channel in the map

        if (it == __channels.end()) {
            std::cout << "ERR_NOSUCHCHANNEL(403)\n";
            return ;//TODO: continue or return?
        }
        
        Channel channel = it->second;
        if (isInChannel(channel, fd) == false)
        {
            std::cout << "ERR_CANNOTSENDTOCHAN(404)\n";
            return ;
        }
        std::map<int, Client>::const_iterator it2 = channel.getChannelClients().begin();
        std::cout << "353 " << channel.getChannelName() << " :\n";
        for (; it2 != channel.getChannelClients().end(); ++it2)
        {
            std::cout << "\t" << it2->second.getUsername() << "\n";
        }
    }
}


void    Server::parseList(std::vector<std::string> &vec, int fd)
{
    if (vec.size() > MAXPARAMS)
    {
        std::cout << "ERR_TOOMANYARGUMENTS(461)\n";// TODO: check if it's the right error code
        return ;
    }
    if (vec.size() == 0)
    {
        std::map<std::string, Channel>::iterator it = __channels.begin();
        std::cout << "321 " << "Channel :Users Name" << std::endl;
        for (; it != __channels.end(); ++it)
        {
            std::cout << "322 " << it->second.getChannelName() << " " << it->second.getChannelClients().size() << " :" << it->second.getChannelTopic() << std::endl;
        }
        std::cout << "323 " << "End of /LIST" << std::endl;
        return ;
    }
    
    std::vector<std::string> chns = split(vec[0], ',');

    for (size_t i = 0; i < chns.size(); ++i) {
        std::map<std::string, Channel>::iterator it = __channels.find(chns[i]);// search for channel in the map

        if (it == __channels.end()) {
            std::cout << "ERR_NOSUCHCHANNEL(403)\n";
            return ;//TODO: continue or return?
        }
        
        Channel channel = it->second;
        if (channel.getChannelType() == 1 && isInChannel(channel, fd) == false)
        {
            std::cout << "ERR_CANNOTSENDTOCHAN(404)\n";
            return ;
        }
        std::cout << "322 " << channel.getChannelName() << " " << channel.getChannelClients().size() << " :" << channel.getChannelTopic() << std::endl;
    }
}