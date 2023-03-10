# include "../include/utils.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"


void    sendMessage(int fd, std::string msg)
{
    if ( send(fd, msg.c_str(), msg.size(), 0) == -1)
        throw std::runtime_error("error: send() failed");
}

std::string backslashR(const std::string& str)
{
    std::string ret;
    for(size_t i = 0 ; i < str.size();i++)
    {
        if(str[i] != '\r')
            ret.push_back(str[i]);
    }
    if(ret[ret.size() - 1]  == '\n')
        return(ret.substr(0, ret.size() - 1));
    else
        return(ret);
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string>    split(std::string &line, char c)
{
    std::vector<std::string>    vec;
    size_t                      pos = 0;
    std::string                 str;

    while ((pos = line.find(c)) != std::string::npos)
    {
        str = line.substr(0, pos);
        vec.push_back(str);
        line.erase(0, pos + 1);
    }
    vec.push_back(line);
    return (vec);
}

