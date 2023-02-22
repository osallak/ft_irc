# include "Channel.hpp"

Channel::Channel() : __channelName(""), __channelTopic(""), __channelClients(), __channelModerator(), __channelType(0) 
{

}

Channel::Channel(std::string channelName, int channelModerator, int channelType) : __channelName(channelName), __channelTopic(""), __channelClients(), __channelModerator(channelModerator), __channelType(channelType)
{

}

Channel::Channel(const Channel &copy) : __channelName(copy.__channelName), __channelTopic(copy.__channelTopic), __channelClients(copy.__channelClients), __channelModerator(copy.__channelModerator), __channelType(copy.__channelType)
{

}

Channel &Channel::operator=(const Channel &copy)
{
    if (this != &copy)
    {
        __channelName = copy.__channelName;
        __channelTopic = copy.__channelTopic;
        __channelClients = copy.__channelClients;
        __channelModerator = copy.__channelModerator;
        __channelType = copy.__channelType;
    }
    return (*this);
}

Channel::~Channel()
{

}

std::string Channel::getChannelName() const
{
    return (__channelName);
}

std::string Channel::getChannelTopic() const
{
    return (__channelTopic);
}

std::string Channel::getChannelPassword() const
{
    return (__channelPassword);
}


// std::map<int, Client> Channel::getChannelClients() const
// {
//     return (__channelClients);
// }

std::map<int, Client> Channel::getChannelInvited() const
{
    return (__channelInvited);
}

std::vector<int>& Channel::getChannelModerator()
{
    return (__channelModerator);
}

int Channel::getChannelType() const
{
    return (__channelType);
}

int Channel::getChannelPass() const
{
    return (__channelPass);
}

void Channel::setChannelName(const std::string& channelName)
{
    __channelName = channelName;
}

void Channel::setChannelTopic(const std::string& channelTopic)
{
    __channelTopic = channelTopic;
}

void Channel::setChannelPassword(const std::string& channelPassword)
{
    __channelPassword = channelPassword;
}


// void Channel::setChannelClients(const std::map<int, Client>& channelClients)
// {
//     __channelClients = channelClients;
// }

void Channel::setChannelInvited(const std::map<int, Client>& channelInvited)
{
    __channelInvited = channelInvited;
}

void Channel::setChannelModerator(int& channelModerator)
{
    __channelModerator.push_back(channelModerator);
}

void Channel::setChannelType(const int& channelType)
{
    __channelType = channelType;
}
int Channel::getChannelClient(std::string __UserName) const
{
    std::map<int, Client>::const_iterator it;
    for (it = BigenIterator(); it != EndIterator(); ++it) {
        if(it->second.getUsername() == __UserName)
            return(it->first);
    }
    return(-1);
}

int Channel::getChannelClientt(int fd) const
{
    std::map<int, Client>::const_iterator it;
    it = __channelClients.find(fd);
    if (it != __channelClients.end())
        return(1);
    return(-1);
}

void Channel::setChannelClients(int __UserId , std::string __UserName)
{
    __channelClients[__UserId].setUsername(__UserName);
}

void Channel::setChannelPass(const int& channelPass)
{
    __channelPass = channelPass;
}
bool Channel::getInvited(int UserId)
{
    if(__channelInvited.find(UserId) != __channelInvited.end())
        return(1);
    else
        return(0);
}
void Channel::SetInviteds(int __UserId, Client User)
{
    __channelInvited[__UserId] = User;
}
size_t                Channel::getClientNb()
{
    return(__channelClients.size());
}
std::map<int, Client>::const_iterator Channel::BigenIterator() const
{
    return(__channelClients.begin());
}
std::map<int, Client>::const_iterator Channel::EndIterator() const
{
    return(__channelClients.end());

}


// Path: src/Channel.cpp
// created: 2023/02/19 5:14:09 by osallak
