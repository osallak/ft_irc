/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 22:04:21 by osallak           #+#    #+#             */
/*   Updated: 2023/02/20 11:27:14 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"
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
                __pollfds.push_back(__NewClient); // add the new client socket to the pollfds vector
                
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
                        if (tmpBuffer.find('\n') != std::string::npos)
                        {
                            __users[__pollfds[i].fd].setCommand(tmpBuffer);
                            // TODO: parse the command
                        }
                        __users[__pollfds[i].fd].appendBuffer(tmpBuffer);
                    }
                    else
                    {
                        // this means the client is not authenticated yet
                    }
                    std::cout << "Client: \t" << buffer << std::endl;
                }
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
