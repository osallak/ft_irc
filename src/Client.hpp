/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 00:52:38 by osallak           #+#    #+#             */
/*   Updated: 2023/02/18 09:10:46 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "utils.hpp"


class Client {
    private:
        std::string     __nickname;
        std::string     __username;
        struct pollfd   __pollfd;
        bool           __isLogged;// true if the client is logged in with a nickname and a correct password 
                                  // this will be used if the make sure that the client is sending commands
                                  // and not just trying to connect to the server (sending password is not like sending a regular command)
    public:
        Client();
        ~Client();
        Client(const Client &copy);
        Client &operator=(const Client &copy);

        std::string     getNickname() const;
        void            setNickname(std::string nickname);
        std::string     getUsername() const;
        void            setUsername(std::string username);
        struct pollfd   getPollfd() const;
        void            setPollfd(struct pollfd pollfd);
        bool            getIsLogged() const;
        void            setIsLogged(bool isLogged);
};
#endif