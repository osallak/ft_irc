#include"Client.hpp"
# include "../include/utils.hpp"

Client::Client() {
    __nickname = "";
    __username = "";
    __isLogged = false;
}

Client::~Client() {
}

Client::Client(const Client &copy) {
    *this = copy;
}

Client &Client::operator=(const Client &copy) {
    if (this != &copy) {
        __nickname = copy.__nickname;
        __username = copy.__username;
        __isLogged = copy.__isLogged;
    }
    return *this;
}

std::string Client::getNickname() const {
    return __nickname;
}

void Client::setNickname(std::string nickname) {
    __nickname = nickname;
}

std::string Client::getUsername() const {
    return __username;
}

void Client::setUsername(std::string username) {
    __username = username;
}

void Client::setPollfd(struct pollfd pollfd) {
    __pollfd = pollfd;
}

bool Client::getIsLogged() const {
    return __isLogged;
}

void Client::setIsLogged(bool isLogged) {
    __isLogged = isLogged;
}

void  Client::appendBuffer(const std::string& buffer) {
    __buffer += buffer;
}

void Client::setCommand(const std::string& command) {
    __command = command;
}

std::string Client::getCommand() const {
    return __command;
}

std::string getCommandWithoutParams(std::string command) {
    std::string commandWithoutParams = command;
    size_t pos = commandWithoutParams.find(" ");
    if (pos != std::string::npos)
        commandWithoutParams = commandWithoutParams.substr(0, pos);
    return commandWithoutParams;
}

int Client::getCommandType() const {
    std::string command = getCommandWithoutParams(__command);//get command without prefix (lowercase)

    if (command == "join")
        return JOIN;
    else if (command == "part")
        return PART;
    else if (command == "ping")
        return PING;
    else if (command == "pong")
        return PONG;
    else if (command == "privmsg")
        return PRIVMSG;
    else if (command == "quit")
        return QUIT;
    else if (command == "kick")
        return KICK;
    else if (command == "mode")
        return MODE;
    else if (command == "invite")
        return INVITE;
    else if (command == "topic")
        return TOPIC;
    else if (command == "names")
        return NAMES;
    else if (command == "list")
        return LIST;
    else
        return UNKNOWN;
}