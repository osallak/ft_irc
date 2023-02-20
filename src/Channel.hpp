# pragma once

# include "../include/utils.hpp"
# include "Client.hpp"

class Channel{
    private:
        std::string           __channelName;
        std::string           __channelTopic;
        std::map<int, Client> __channelClients;
        int                    __channelModerator;
        int                  __channelType;//0 = public, 1 = private
        std::string           __channelPassword;

    public:
        Channel();
        Channel(std::string channelName, int channelModerator, int channelType);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &copy);
        ~Channel();

        std::string           getChannelName() const;
        std::string           getChannelTopic() const;
        std::map<int, Client> getChannelClients() const;
        int                   getChannelModerator() const;
        int                   getChannelType() const;

        void setChannelName(const std::string& channelName);
        void setChannelTopic(const std::string& channelTopic);
        void setChannelClients(const std::map<int, Client>& channelClients);
        void setChannelModerator(const int& channelModerator);
        void setChannelType(const int& channelType);

};

