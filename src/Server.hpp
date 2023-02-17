/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 22:25:51 by osallak           #+#    #+#             */
/*   Updated: 2023/02/17 22:31:07 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "utils.hpp"

class Server{
    private:
        int         __port;
        std::string __password;

    public:

        Server();
        ~Server();
        Server(const Server &copy);
        Server &operator=(const Server &copy);

        int         getPort() const;
        void        setPort(int port);
        std::string getPassword() const;
        void        setPassword(std::string password);
        bool        connect( void );
        void        disconnect( void );
        int         authentification( void );
        std::string getCommand( void );
};
