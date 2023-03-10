# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "../include/utils.hpp"

void    Server::parseCommand( int fd )
{
    std::string                 line;
    std::string                 command;
    std::vector<std::string>    res;
    size_t                      pos = 0;
    std::string                 str;

    line = __users[fd].getBuffer();
    while ((pos = line.find(' ')) != std::string::npos)
    {
        str = line.substr(0, pos);
        res.push_back(str);
        line.erase(0, pos + 1);

    }
    line = line.substr(0,line.size() - 1);
    res.push_back(line);
    command = res[0];
    for (size_t i = 0; i < command.size(); ++i){
        command[i] = (char)(tolower(command[i]));
    }
    res.erase(res.begin());


    if (command == KICK)
        parseKick(res, fd);
    else if (command == "invite")
        parseInvite(res, fd);
    else if (command == MODE)
        parseMode(res, fd);
    else if (command == TOPIC)
        parseTopic(res, fd);
    else if (command == QUIT)
        parseQuit(fd);
    else if (command == PART)
        parsePart(res, fd);
    else if (command == NAMES)
        parseNames(res, fd);
    else if (command == LIST)
        parseList(res, fd);
    else if (command == PRIVMSG)
        parsePrivmsg(res, fd);
    else if(command == "nick")
        parseNick(res, fd);
    else if (command == JOIN)
        parseJoin(res, fd);
    else if (command == "/joke" || command == "/time")
        runBot(command, fd);
    else if (command == "notice")
        parseNotice(res, fd);
    else if (command == "ping"){
        parsePing(res, fd);
    }else {
        std::string msg = "421 " + __users[fd].getNickname() + " " + command + " :Unknown command\n";
        sendMessage(fd,msg);
    }
}