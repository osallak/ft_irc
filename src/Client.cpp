#include"Client.hpp"


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
void            Client::setIsbuffer(std::string buffer)
{
    __buffer = buffer;
}
std::string     Client::getIsbuffer()
{
    return(__buffer);
}
void            Client::setIspassword(std::string password)
{
    __password = password;
}
std::string     Client::getIspassword()
{
    return(__password);
}

