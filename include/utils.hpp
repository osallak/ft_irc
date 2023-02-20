/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aanjaimi <aanjaimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 22:44:09 by osallak           #+#    #+#             */
/*   Updated: 2023/02/20 11:19:01 by aanjaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <algorithm>
# include <iostream>
# include <string>
# include <vector>
# include <map>
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


# define KICK "kick"
# define MODE "mode"
# define INVITE "invite"
# define TOPIC "topic"
# define PING "ping"
# define PONG "pong"
# define QUIT "quit"
# define ERROR "error"
# define PART "part"
# define NAMES "names"
# define LIST "list"
# define PRIVMSG "privmsg"
# define JOIN "join"

// this file is for the common functions, classes, etc. that are used in multiple files
// for example, the error functions...