# pragma once

# include "../include/utils.hpp"
# include  "Client.hpp"

// this class should be inherited by every single command class (NICK, JOIN, etc...)
//NOTE: this class is abstract and should not be instantiated
class Command{
    protected:
        std::string                    _command;
        std::vector <std::string>      _params;
        std::string                    _prefix;
        std::string                    _fullCommand;
        std::string                    _fullCommandNoPrefix;
        int                            _commandType;

    public:
        Command(std::string fullCommand);
        // Command(std::string command, std::string params, std::string prefix);
        Command(const Command &copy);
        Command &operator=(const Command &copy);
        ~Command();

        const std::string &getCommand() const;
        const std::vector <std::string> &getParams() const;
        const std::string &getPrefix() const;
        const std::string &getFullCommand() const;
        const std::string &getFullCommandNoPrefix() const;
        int         getCommandType() const;

        void setCommand(const std::string& command);
        void setParams(const std::vector <std::string>& params);
        void setPrefix(const std::string& prefix);
        void setFullCommand(const std::string& fullCommand);
        void setFullCommandNoPrefix(const std::string& fullCommandNoPrefix);
        void setCommandType(const int& commandType);

        virtual void parseCommand() = 0;
        // virtual void executeCommand(Client client, std::vector<Client> &clients, std::vector<Channel> &channels);// idk we may need this

};//this class is used to parse the command and store it in a Command object
    // it should be inherited by every single command class (NICK, JOIN, etc...)