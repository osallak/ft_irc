#include"Client.hpp"
# include "../include/utils.hpp"

Client::Client() {
    __nickname = "";
    __username = "";
    __password = "";
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
void            Client::setBuffer(std::string buffer)
{
    __buffer = buffer;
}
std::string     Client::getBuffer()
{
    return(__buffer);
}
void            Client::setPassword(std::string password)
{
    __password = password;
}
std::string     Client::getPassword()
{
    return(__password);
}

