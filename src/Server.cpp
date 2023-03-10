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

Server::Server() : __port(-1), __password("")
{
    char hostname[_SC_HOST_NAME_MAX];
    gethostname(hostname, _SC_HOST_NAME_MAX);
    this->__hostname = hostname;
}

Server::~Server()
{
    //close sockets and stuff etc...
    for (std::map<int, Client>::iterator it = __NewConnections.begin(); it != __NewConnections.end(); it++) {
        close(it->first);
    }
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
                if (valread == 0) // if the client disconnected
                {
                    DeleteUser(__pollfds[i].fd);
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
                            {
                                CurrentBuffer = backslashR(CurrentBuffer);
                                SetUserInf(ParceConnection(CurrentBuffer), __pollfds[i].fd);
                            }
                            else
                            {
                                std::vector<std::pair<std::string, std::string> > cmds = ParceConnectionLine(CurrentBuffer);
                                // for (size_t i = 0;i < cmds.size();i++)
                                if(cmds.size() != 3) {
                                    sendMessage(__pollfds[i].fd,":* 667 * :Enter PASS <password>, NICK <nickname>, USER <user>\n");
                                } else {
                                    std::string str = "";
                                    for(size_t i = 0 ; i < cmds[1].second.size();i++)
                                    {
                                        if(cmds[1].second[i] == ' ')
                                            break;
                                        else
                                            str.push_back(cmds[1].second[i]);
                                    }
                                    __NewConnections.find(__pollfds[i].fd)->second.setPassword(cmds[0].second);
                                    __NewConnections.find(__pollfds[i].fd)->second.setUsername(str);
                                    __NewConnections.find(__pollfds[i].fd)->second.setNickname(cmds[2].second);
                                    __users[__pollfds[i].fd] = __NewConnections.find(__pollfds[i].fd)->second;
                                    std::string msg = ":" + __hostname + " 001 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() +  " :Welcome to the Internet Relay Network " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + "!~" + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + "@" + "127.0.0.1\r\n";
                                    msg += ":" + __hostname + " 002 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :Your host is " + __hostname + ", running version leet-irc 1.0.0\r\n";
                                    msg += ":" + __hostname + " 003 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :This server has been started Wed Oct 12 2022\r\n";
                                    msg += ":" + __hostname + " 004 " +  __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " " + __hostname + " leet-irc 1.0.0 aioOrsw aovimntklbeI\r\n";
                                    msg += ":" + __hostname + " 251 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :There are 2 users and 0 services on 1 servers\r\n";
                                    msg += ":" + __hostname + " 375 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :- " + __hostname + " Message of the day -\r\n";
                                    msg += ":" + __hostname + " 376 " + __NewConnections.find(__pollfds[i].fd)->second.getNickname() + " :End of MOTD command\r\n";
                                    sendMessage(__pollfds[i].fd,msg);
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

std::string Server::GetUserName(int fd)
{
    std::map<int, Client>::iterator it;
    std::string                     message;
    it = __users.find(fd);
    if (it != __users.end())
    {
        return (it->second.getNickname());
    }
    message = ":" + GetUserName(fd) + " 401 * :No such nick/channel\n";
    sendMessage(fd, message);
    return ("EROOR");
}

int   Server::IsModerator(Channel &channel, std::string username)
{
    std::vector<int>::iterator  it_vec;

    if (std::find(channel.getChannelModerator().begin(), channel.getChannelModerator().end(), GetUserId(username)) != channel.getChannelModerator().end())
        return (1);
    return (-1);
}

void  Server::RemoveModerator(Channel &channel, std::string username)
{
    std::vector<int>::const_iterator it = std::find(channel.getChannelModerator().begin(), channel.getChannelModerator().end(), GetUserId(username));
    if (it != channel.getChannelModerator().end())
        channel.getChannelModerator().erase(it);
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

std::string Server::getHostname() const
{
    return __hostname;
}
