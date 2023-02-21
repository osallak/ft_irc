























































































































































































































































































































































































































































































































# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
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
        if(__NewConnections.find(UserId)->second.getIspassword().empty())
        {
            if(cmd.first == "password" && cmd.second == __password)
                __NewConnections.find(UserId)->second.setIspassword(cmd.second);
            else
                std::cout << "Error45\n";
        }
        else if(__NewConnections.find(UserId)->second.getUsername().empty())
        {
            if(cmd.first == "username")
                 __NewConnections.find(UserId)->second.setUsername(cmd.second);
            else
                std::cout << "Error46\n";
        }
        else if(__NewConnections.find(UserId)->second.getNickname().empty())
        {
            if(cmd.first == "nickname")
            {
                 __NewConnections.find(UserId)->second.setNickname(cmd.second);
                 __NewConnections.find(UserId)->second.setIsLogged(true);
                 __users[UserId] = __NewConnections.find(UserId)->second;
            }
            else
                std::cout << "Error47\n";
        }
    }
    else
        std::cout << "Error98\n";
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
    if(ret.first != "password" && ret.first != "username" && ret.first != "nickname")
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
    // int port_int = std::stoi(port);
    if (port.empty() || port.size() > 5 || atoi(port.c_str()) > 65535 || atoi(port.c_str()) < 0)
    {
        std::cerr << "Error: invalid port" << std::endl;
        exit(1);
    }
    for (unsigned int i  = 0; i < port.size(); i++)
    {
        if (!isdigit(port[i]))
        {
            std::cerr << "Port must be a number" << std::endl;
            exit(1);
        }
    }
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

    if(send(UserId,"channels can you access\n",25,0) == -1)
    {
        std::cout << "send() Failed\n";
                exit(0);
    }
    std::map<std::string, Channel>::iterator it;
    for (it = __channels.begin(); it != __channels.end(); ++it) {
        if(it->second.getInvited(UserId))
        {
            if (send(UserId,it->second.getChannelName().c_str(),it->second.getChannelName().size(), 0) == - 1)
            {
                std::cout << "send() Failed\n";
                exit(0);
            }
        }
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
    if(__arg.size() == 1)
        __ValRead = send(__UserId,"341\n",5, 0);
    else if(__ReceiverId == -1)
        __ValRead = send(__UserId,"403\n",5, 0);
    else if(__channels.find(__arg[1]) == __channels.end())
        __ValRead = send(__UserId,"403",4, 0);
    else if(!__channels[__arg[1]].getClientNb())
        __ValRead = send(__UserId,"482\n",5, 0);
    else if(!__channels[__arg[1]].getInvited(__UserId))
        __ValRead = send(__UserId,"442\n",5, 0);
    else
    {
        __ValRead = send(__UserId,"341\n",5, 0);
        __channels[__arg[1]].SetInviteds(__ReceiverId, __users[__UserId]);
        // __channels[__arg[1]].setChannelClients(__ReceiverId , __arg[0]);
    }
    if(__ValRead == - 1)
        std::cout << "send() failde\n";

}

void Server::parseTopic(std::vector<std::string>__arg,int __UserId)
{
    int __ValRead = 0;
    std::vector<int> vec = __channels.find(__arg[1])->second.getChannelModerator();
    if(!__arg.size())
        std::cout << "NO arg\n";
    else if(__arg.size() == 1)
        __ValRead  = send(__UserId,"331\n",5, 0);
    else if(__channels.find(__arg[1]) == __channels.end())
        __ValRead = send(__UserId,"403",4, 0);
    else if(__channels.find(__arg[1])->second.getChannelType() && std::find(vec.begin(),vec.end(),__UserId) != vec.end())
        __ValRead = send(__UserId,"482",4, 0);
    else
    {
        std::map<int, Client>::const_iterator BeginIt;
        std::map<int, Client>::const_iterator EndIt;
        BeginIt = __channels.find(__arg[1])->second.BigenIterator();
        EndIt = __channels.find(__arg[1])->second.EndIterator();
        while(BeginIt != EndIt)
        {
            size_t len = __channels.find(__arg[1])->second.getChannelTopic().size();
             __channels.find(__arg[1])->second.setChannelTopic(__arg[2]);
            __ValRead = send(BeginIt->first,__channels.find(__arg[1])->second.getChannelTopic().c_str(),len, 0);
            __ValRead = send(BeginIt->first,"\n",1, 0);
            BeginIt++;
        }
    }
    if(__ValRead == - 1)
        std::cout << "send() failde\n";
}

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

        
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
    
    int timeout = (1000 * 60); // 1 minute
    // infinite loop to keep the server running

    while (true)
    {
        int ret;
        //pollfd is a struct that contains the file descriptor, the events to check for, and the revents that occured
        // it may be changed if some client disconnects/connects
        // figure out how to add the client sockets to the pollfds vector sumultaneously
        if ( (ret = poll(__pollfds.data(), __pollfds.size(), timeout)) < 0)
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
                        std::string CurrentBuffer = __users.find(__pollfds[i].fd)->second.getIsbuffer();
                        CurrentBuffer+=buffer;
                        __users.find(__pollfds[i].fd)->second.setIsbuffer(CurrentBuffer);
                        if(CurrentBuffer.find("\n") != std::string::npos)
                        {
                            parseCommand(__pollfds[i].fd);
                            __users.find(__pollfds[i].fd)->second.setIsbuffer("");
                        }
                    }
                    else
                    {
                        std::string CurrentBuffer = __NewConnections.find(__pollfds[i].fd)->second.getIsbuffer();
                        CurrentBuffer+=buffer;
                        __NewConnections.find(__pollfds[i].fd)->second.setIsbuffer(CurrentBuffer);
                        CurrentBuffer =   __NewConnections.find(__pollfds[i].fd)->second.getIsbuffer();
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
                                    std::cout << "Error110\n";
                                else
                                {
                                    __NewConnections.find(__pollfds[i].fd)->second.setIspassword(cmds[0].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setUsername(cmds[1].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setUsername(cmds[2].second);
                                }

                            }
                            __NewConnections.find(__pollfds[i].fd)->second.setIsbuffer("");
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

    line = __users[fd].getIsbuffer();
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
        std::cout << res[i] << std::endl;
    // (void)fd;
    std::cout << "Cmd ------>" << command  << "|"<< std::endl;
    std::cout << res.size() << std::endl;
    // if (command == KICK)
    //     parseKick(res, fd);
     if (command == "invite")
        parseInvite(res, fd);
    // else if (command == MODE)
    //     parseMode(res, fd);
    else if (command == TOPIC)
        parseTopic(res, fd);
    // else if (command == PING)
    //     parsePing(res, fd);
    // else if (command == PONG)
    //     parsePong(res, fd);
    // else if (command == QUIT)
    //     parseQuit(res, fd);
    // else if (command == ERROR)
    //     parseError(res, fd);
    // else if (command == PART)
    //     parsePart(res, fd);
    // else if (command == NAMES)
    //     parseNames(res, fd);
    // else if (command == LIST)
    //     parseList(res, fd);q
    // else if (command == PRIVMSG)
    //     parsePrivmsg(res, fd);
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
    // size_t i;
    std::cout << "hello\n";
    (void)fd;
    if (vec.size() <= 1)
        std::cout << "ERR_NEEDMOREPARAMS(461)\n";
        return ;
    }
    chn = split(vec[0], ',');
    key = split(vec[1], ',');
    //check if channels exist
    for (i = 0;i < chn.size(); ++i)
    {
        it = __channels.find(chn[i]);
        if (it == __channels.end())
        {
            __channels[chn[i]] = Channel(chn[i], fd, 0);
            std::cout << "Channel creted succesfully\n";
            return ;
        }
        //check if it's private
        if (it->second.getChannelType() == 1)
        {
            inv = it->second.getChannelInvited().find(fd);
            if (inv == it->second.getChannelInvited().end())
            {
                std::cout << "ERR_INVITEONLYCHAN(473)\n";
                return ;
            }
            if (it->second.getChannelPass() == 1)
            {
                if (it->second.getChannelPassword() != key[k++])
                {
                    std::cout << "ERR_BADCHANNELKEY(475)\n";
                    return ;
                }
                else
                {
                    std::cout << "Joined\n";
                    return ;
                }
            }
        }
        else
        {
            if (it->second.getChannelPass() == 1)
            {
                if (it->second.getChannelPassword() != key[k++])
                {
                    std::cout << "ERR_BADCHANNELKEY(475)\n";
                    return ;
                }
                else
                {
                    std::cout << "Joined\n";
                    return ;
                }
            } else  {
                std::cout << "Joined\n";
                return ;
            }
        }
    }
}

void    Server::parseMode(std::vector<std::string> &vec, int fd)
{
    std::map<int,Client>::iterator  it;
    // size_t                          i;

    if (vec.size() == 0)
    {
        std::cout << "ERR_NEEDMOREPARAMS(461)\n";
        return ;
    }
    (void)fd;
    // it = __users.find()
    // for (i = 0;i < __channels.size();++i)
    // {
    //     if (__channels[vec[0]].getChannelName() == vec[0])
    //         break ;
    // }
    // if (i == __channels.size())
    //     std::cout << "ERR_NOSUCHCHANNEL(403)\n";
    // it = std::find(__channels[vec[0]].getChannelModerator().begin(), __channels[vec[0]].getChannelModerator().end(), fd);
    // if (it == __channels[vec[0]].getChannelModerator().end())
    //     std::cout << "ERR_CHANOPRIVSNEEDED(482)\n";
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
//         std::cout << "ERR_NOSUCHCHANNEL(403)\n";
//     if (__channels[vec[0]].getChannelModerator() != fd)
//         std::cout << "ERR_CHANOPRIVSNEEDED(482)\n";
// }