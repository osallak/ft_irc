/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smazouz <smazouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 22:25:51 by osallak           #+#    #+#             */
/*   Updated: 2023/02/19 22:08:39 by smazouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

// # include "utils.hpp"
# include "Client.hpp"
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
        std::map<int,Client>        __NewConnections;
    public:

        Server();
        ~Server();
        Server(const Server &copy);
        Server &operator=(const Server &copy);

        unsigned short         getPort() const;
        void                   setPort(std::string port);
        std::string            getPassword() const;
        void                   setPassword(std::string password);
        bool                   run( void );
        void                   disconnect( void );
        int                    authentification( void );
        std::string            getCommand( void );
        std::pair<std::string,std::string> ParceConnection(std::string cmd);
        std::vector<std::pair<std::string, std::string> > ParceConnectionLine(std::string cmd);
        void                    SetUserInf(std::pair<std::string,std::string> cmd, int UserId);

};


/*


*/