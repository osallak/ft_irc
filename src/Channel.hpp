# pragma once

# include "../include/utils.hpp"
# include "Client.hpp"

class Channel{
    private:
        std::string           __channelName;
        std::string           __channelTopic;
        std::map<int, std::string> __channelClients;
        std::set<int>         __Invited;
        int                   __channelModerator;
        int                   __channelType;//0 = public, 1 = private
        std::string           __channelPassword;

    public:
        Channel();
        Channel(std::string channelName, int channelModerator, int channelType);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &copy);
        ~Channel();

        std::string                getChannelName() const;
        std::string                getChannelTopic() const;
        int                        getChannelClients(std::string __UserName) const;
        int                        getChannelModerator() const;
        int                        getChannelType() const;
        std::map<int, std::string> getChannelClientsMap() const;

        void setChannelName(const std::string& channelName);
        void setChannelTopic(const std::string& channelTopic);
        void setChannelClients(int __userId , std::string __UserName);
        void setChannelModerator(const int& channelModerator);
        void setChannelType(const int& channelType);

};

