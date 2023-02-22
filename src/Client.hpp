#ifndef CLIENT_HPP
# define CLIENT_HPP

// # include "utils.hpp"
// # include "Server.hpp"
# include <iostream>
#include<poll.h>



class Client {
    private:
        std::string     __password;
        std::string     __nickname;
        std::string     __username;
        struct pollfd   __pollfd;
        bool           __isLogged;
        std::string     __buffer;
        std::string     __command;
        
    public:
        Client();
        ~Client();
        Client(const Client &copy);
        Client &operator=(const Client &copy);

        std::string     getNickname() const;
        void            setNickname(std::string nickname);
        std::string     getUsername() const;
        void            setUsername(std::string username);
        int             getFd() const;
        void            setPollfd(struct pollfd pollfd);
        bool            getIsLogged() const;
        void            setIsLogged(bool isLogged);
        void            appendBuffer(const std::string& buffer);
        void            setCommand(const std::string& command);
        std::string     getCommand() const;
        int             getCommandType() const;
        void            setIsbuffer(std::string buffer);
        std::string     getIsbuffer();
        void            setIspassword(std::string password);
        std::string     getIspassword();
};
#endif