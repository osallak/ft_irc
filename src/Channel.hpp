# pragma once

# include "../include/utils.hpp"
# include "Client.hpp"

class Channel{
    private:
        std::string           __channelName;
        std::string           __channelTopic;
        std::map<int, Client> __channelClients;
        std::map<int, Client>        __channelInvited;
        int                   __channelModerator;
        int                   __channelType;//0 = public, 1 = private
        std::string           __channelPassword;

    public:
        Channel();
        Channel(std::string channelName, int channelModerator, int channelType);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &copy);
        ~Channel();

        std::string           getChannelName() const;
        std::string           getChannelTopic() const;
        int                   getChannelClients(std::string __UserName) const;
        int                   getChannelModerator() const;
        int                   getChannelType() const;
        bool                  getInvited(int UserID);
        size_t                getClientNb();
        std::map<int, Client>::const_iterator BigenIterator() const;
        std::map<int, Client>::const_iterator EndIterator() const;

        

        void SetInviteds(int __UserID, Client User);
        void setChannelName(const std::string& channelName);
        void setChannelTopic(const std::string& channelTopic);
        void setChannelClients(int __userId , std::string __UserName);
        void setChannelModerator(const int& channelModerator);
        void setChannelType(const int& channelType);

};

