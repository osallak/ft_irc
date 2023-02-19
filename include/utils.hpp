/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osallak <osallak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 22:44:09 by osallak           #+#    #+#             */
/*   Updated: 2023/02/19 23:32:33 by osallak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

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



enum {
    PART,
    JOIN,
    PING,
    PONG,
    PRIVMSG,
    QUIT,
    UNKNOWN,
    KICK,
    MODE,
    INVITE,
    TOPIC,
    NAMES,
    LIST,
};

/*
PING
PONG
QUIT
ERROR
PART
NAMES
 LIST
PRIVMSG
JOIN

KICK
MODE
INVITE
TOPIC

*/
// this file is for the common functions, classes, etc. that are used in multiple files
// for example, the error functions...