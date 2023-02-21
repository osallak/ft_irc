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

std::map<int, Client> Channel::getChannelClients() const
{
    return (__channelClients);
}

std::map<int, Client> Channel::getChannelInvited() const
{
    return (__channelInvited);
}

std::vector<int> Channel::getChannelModerator() const
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

void Channel::setChannelClients(const std::map<int, Client>& channelClients)
{
    __channelClients = channelClients;
}

void Channel::setChannelInvited(const std::map<int, Client>& channelInvited)
{
    __channelInvited = channelInvited;
}

void Channel::setChannelModerator(const std::vector<int>& channelModerator)
{
    __channelModerator = channelModerator;
}

void Channel::setChannelType(const int& channelType)
{
    __channelType = channelType;
}

void Channel::setChannelPass(const int& channelPass)
{
    __channelPass = channelPass;
}

// Path: src/Channel.cpp
// created: 2023/02/19 5:14:09 by osallak
