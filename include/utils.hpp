# pragma once

# include <algorithm>
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <set>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <poll.h>
# include <signal.h>
# include <set>

# define KICK "kick"
# define MODE "mode"
# define INVITE "invite"
# define TOPIC "topic"
# define QUIT "quit"
# define ERROR "error"
# define PART "part"
# define NAMES "names"
# define LIST "list"
# define PRIVMSG "privmsg"
# define JOIN "join"

# define CHANNEL_PREFIX '#'
# define PLUS '+'
# define MINUS '-'
# define MOD 'm'
# define KEY 'k'
# define INV 'i'
# define CHANNEL_PREFIX '#'
# define MAXPARAMS 14

void        sendMessage(int fd, std::string msg);
std::string backslashR(const std::string& str);
std::string trim(const std::string& str);
std::vector<std::string>    split(std::string &line, char c);