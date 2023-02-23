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
        std::map<int, Client> getChannelInvited() const;
        std::vector<int>      getChannelModerator() const;
        int                   getChannelType() const;
        int                   getChannelPass() const;
        std::string           getChannelPassword() const;
        bool                  getInvited(int UserID);
        size_t                getClientNb();
        int                   getChannelClient(std::string __UserName) const;
        std::map<int, Client>::const_iterator BigenIterator() const;
        std::map<int, Client>::const_iterator EndIterator() const;

        
        const std::map<int, Client>& getChannelClients() const;//return __channelClients map, needed to check if user is in channel

        void SetInviteds(int __UserID, Client User);
        void setChannelName(const std::string& channelName);
        void setChannelTopic(const std::string& channelTopic);
        void setChannelInvited(const std::map<int, Client>& channelInvited);
        // void setChannelModerator(const std::vector<int>& channelModerator);
        void setChannelType(const int& channelType);
        void setChannelPass(const int& channelPass);
        void setChannelPassword(const std::string& channelPassword);
        void setChannelClients(int __UserId , std::string __UserName);
        void setChannelModerator(int& channelModerator);
        void eraseClient(int __UserId);
        void eraseInvited(int __UserId);

};

