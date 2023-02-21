# pragma once

# include "../include/utils.hpp"
# include "Client.hpp"

class Channel{
    private:
        std::string           __channelName;
        std::string           __channelTopic;
        std::map<int, Client> __channelClients;
        std::map<int, Client> __channelInvited;
        std::vector<int>      __channelModerator;
        int                  __channelType;//0 = public, 1 = private
        int                  __channelPass;//0 = without, 1 = with
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
        std::map<int, Client> getChannelInvited() const;
        std::vector<int>      getChannelModerator() const;
        int                   getChannelType() const;
        int                   getChannelPass() const;
        std::string           getChannelPassword() const;

        void setChannelName(const std::string& channelName);
        void setChannelTopic(const std::string& channelTopic);
        void setChannelClients(const std::map<int, Client>& channelClients);
        void setChannelInvited(const std::map<int, Client>& channelInvited);
        void setChannelModerator(const std::vector<int>& channelModerator);
        void setChannelType(const int& channelType);
        void setChannelPass(const int& channelPass);
        void setChannelPassword(const std::string& channelPassword);

};

