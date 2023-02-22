# include "../include/utils.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"


void    listServerUsers(std::map<int, Client> const& users, int fd)
{
    std::map<int, Client>::const_iterator it = users.begin();
    std::map<int, Client>::const_iterator it2 = users.end();
    for (; it != it2; it++)
    {
        std::cout << "User: " << it->second.getUsername() <<  " Nickname: " << it->second.getNickname() << std::endl;
        // send this to client fd
    }
    (void)fd;
}