# include "Command.hpp"

Command::Command(std::string fullCommand) {
    this->__fullCommand = fullCommand;
    //other attributes are set in parseCommand
    this->__fullCommandNoPrefix = "";
    this->__commandType = 0;
    this->__command = "";
    this->__prefix = "";
    this->__params.clear();
}

Command::Command(const Command &copy) {
    *this = copy;
}

Command &Command::operator=(const Command &copy) {
    if (this != &copy) {
        this->__command = copy.__command;
        this->__params = copy.__params;
        this->__prefix = copy.__prefix;
        this->__fullCommand = copy.__fullCommand;
        this->__fullCommandNoPrefix = copy.__fullCommandNoPrefix;
        this->__commandType = copy.__commandType;
    }
    return (*this);
}

Command::~Command() {
    //close sockets and stuff etc...
}

const std::string &Command::getCommand() const {
    return (this->__command);
}

const std::vector <std::string> &Command::getParams() const {
    return (this->__params);
}

const std::string &Command::getPrefix() const {
    return (this->__prefix);
}

const std::string &Command::getFullCommand() const {
    return (this->__fullCommand);
}

const std::string &Command::getFullCommandNoPrefix() const {
    return (this->__fullCommandNoPrefix);
}

int Command::getCommandType() const {
    return (this->__commandType);
}

void Command::setCommand(const std::string& command) {
    this->__command = command;
}

void Command::setParams(const std::vector <std::string>& params) {
    this->__params = params;
}

void Command::setPrefix(const std::string& prefix) {
    this->__prefix = prefix;
}

void Command::setFullCommand(const std::string& fullCommand) {
    this->__fullCommand = fullCommand;
}

void Command::setFullCommandNoPrefix(const std::string& fullCommandNoPrefix) {
    this->__fullCommandNoPrefix = fullCommandNoPrefix;
}

void Command::setCommandType(const int& commandType) {
    this->__commandType = commandType;
}
