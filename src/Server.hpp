# pragma once

// # include "utils.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include <iostream>
# include <vector>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include<map>

class Server{
    private:
        unsigned short            __port;
        std::string                __password;
        int                        __socket;//remove this if it's not used
        struct sockaddr_in         __address;
        // std::vector<Client>        __clients;
        std::vector<struct pollfd> __pollfds;
        struct pollfd              __spollfd;// s stands for server
        std::map<int,Client>        __users;
        std::map<std::string,Channel> __channels;
        std::map<int,Client>        __NewConnections;
        int                         __currentNdx;
        std::string                 __hostname;
    public:

        Server();
        ~Server();
        Server(const Server &copy);
        Server &operator=(const Server &copy);

        std::string            getHostname() const;
        unsigned short         getPort() const;
        void                   setPort(std::string port);
        std::string            getPassword() const;
        void                   setPassword(std::string password);
        bool                   run( void );
        std::pair<std::string,std::string> ParceConnection(std::string cmd);
        std::vector<std::pair<std::string, std::string> > ParceConnectionLine(std::string cmd);
        void                    SetUserInf(std::pair<std::string,std::string> cmd, int UserId);

        // void                        parseKick( std::vector<std::string> &, int);
        void                        parseJoin( std::vector<std::string> &, int);
        void                        parseMode( std::vector<std::string> &, int);
        // void                   setCommand( std::string );
        void                   parseCommand( int fd );
        void                   parseKick( std::vector<std::string> &, int);
        void                   parsePrivmsg( std::vector<std::string> &, int);
        void                   parseInvite(std::vector<std::string>__argm,int __UserId);
        int                   GetUserId(std::string UserName);
        bool                    isInChannel(Channel &channel, int fd) const;
        void                    parseTopic(std::vector<std::string>__arg,int __UserId);
        void                    parsePart(std::vector<std::string>__arg,int __UserId);
        void                    parseQuit(int __UserId);
        void                    DeleteUser(int __Userid);
        void                    parseNames(std::vector<std::string> &, int fd);
        void                    parseList(std::vector<std::string> &, int fd);
        void                    parseNotice(std::vector<std::string> &, int fd);
        void                    runBot(const std::string& ,int);
        void                    parseNick(std::vector<std::string> , int fd);
        std::string             getNicknameById(int fd);
        int                     IsModerator(Channel &channel, std::string username);
        void                    RemoveModerator(Channel &channel, std::string username);
        void                    parsePing(std::vector<std::string> &, int fd);
        void                    displayWelcomeMessage(int fd) const;
};


/*


*/