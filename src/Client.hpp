/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 00:52:38 by osallak           #+#    #+#             */
/*   Updated: 2023/02/19 14:53:11 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP

// # include "utils.hpp"
// # include "Server.hpp"
# include <iostream>
#include<poll.h>



class Client {
    private:
        std::string     __nickname;
        std::string     __username;
        struct pollfd   __pollfd;
        bool           __isLogged;
        std::string     __buffer;
        
    public:
        Client();
        ~Client();
        Client(const Client &copy);
        Client &operator=(const Client &copy);

        std::string     getNickname() const;
        void            setNickname(std::string nickname);
        std::string     getUsername() const;
        void            setUsername(std::string username);
        // struct pollfd   getPollfd() const;
        void            setPollfd(struct pollfd pollfd);
        bool            getIsLogged() const;
        void            setIsLogged(bool isLogged);
};
#endif