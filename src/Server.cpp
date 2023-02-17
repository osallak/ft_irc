/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 22:04:21 by osallak           #+#    #+#             */
/*   Updated: 2023/02/17 22:30:49 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"

/* 
    - every attribute prefixed with __ is private
    - every attribute prefixed with _ is protected
    - every attribute prefixed with nothing is public

    - getters should be const
    - do not change something if some of your team members are working on it
    - make a todo list of what you want to do
    - do not push to master
    - make a local branch for each feature you want to add, bug you want to fix, etc...

*/
Server::Server() : _port(-1), __password("")
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
        this->_port = copy._port;
        this->__password = copy.__password;
    }
    return (*this);
}

int Server::getPort() const
{
    return (this->_port);
}

void Server::setPort(int port)
{
    this->_port = port;
}

std::string Server::getPassword() const
{
    return (this->__password);
}

void Server::setPassword(std::string password)
{
    this->__password = password;
}

bool Server::connect( void )
{
    //connect to the server, return true if success, false if not
    return (true);
}

void Server::disconnect( void )
{
    //disconnect from the server
    // it shoud be called with the client to disconnect as a parameter (or something like that)
}

int Server::authentification( void )
{
    //authentification with the server, return 0 if success, 1 if not
    // it receives the password from the client and compares it to the one in the server
    // if it's the same, it returns 1, else it returns 0
    return (0);
}