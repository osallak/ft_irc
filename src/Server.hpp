/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 22:25:51 by osallak           #+#    #+#             */
/*   Updated: 2023/02/19 22:38:11 by osallak          ###   ########.fr       */
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
        void                   parseCommand( int fd );
};


/*


*/