# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"


void    Server::parsePing(std::vector<std::string> &vec, int fd)
{
    std::string message = "";
    if (vec.size() < 2)
        return ;
    std::string msg = "";
    if (vec[1][0] != ':')
        msg += ":";
    for (size_t i = 1; i < vec.size(); ++i)
    {
        msg += vec[i];
        if (i != vec.size() - 1)
            msg += " ";
    }
    message = "PONG " + msg + "\n";
    sendMessage(fd, message);
}