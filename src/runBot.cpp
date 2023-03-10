# include "Server.hpp"
#include <utility>
#include <cstdlib>//for atoi and stuff...
# include <algorithm>//for std::replace
# include <string>
# include "utils.hpp"

void    insertJokes(std::vector<std::string>& jokes)
{
    jokes.push_back("What do you call a fake noodle? An impasta.");
    jokes.push_back("How many apples grow on a tree? All of them.");
    jokes.push_back("Want to hear a joke about paper? Nevermind it's tearable.");
    jokes.push_back("I just watched a program about beavers. It was the best dam program I've ever seen.");
    jokes.push_back("Why did the coffee file a police report? It got mugged.");
    jokes.push_back("How does a penguin build it's house? Igloos it together.");
    jokes.push_back("Dad, did you get a haircut? No I got them all cut.");
    jokes.push_back("This graveyard looks overcrowded. People must be dying to get in there.");
    jokes.push_back("Did you hear about the actor who fell through the floorboards? He was just going through a stage.");
    jokes.push_back("Did you hear about the claustrophobic astronaut? He just needed a little space.");
    jokes.push_back("Why do you never see elephants hiding in trees? Because they're so good at it.");
    jokes.push_back("Dad, can you put my shoes on? No, I don't think they'll fit me.");
    jokes.push_back("What do you call a belt made out of watches? A waist of time.");
    jokes.push_back("Why did the scarecrow win an award? Because he was outstanding in his field.");
    jokes.push_back("Why don't skeletons ever go trick or treating? Because they have no body to go with.");
    jokes.push_back("What do you call a dog that does magic tricks? A labracadabrador.");
}

void    Server::runBot(const std::string& command, int fd)
{
    std::vector<std::string> jokes;

    insertJokes(jokes);

    std::string message;

    if (command == "/joke")
    {
        std::string  res = "";
        int status = system("which curl > /dev/null");
        if (status != 0){
            res = jokes[rand() % jokes.size()];
        } else {
            status = system("curl -HI --silent  --output /dev/null  \"Accept: text/plain\" https://icanhazdadjoke.com/ > /tmp/.joke");
            if (status != 0) {
                res = jokes[rand() % jokes.size()];
            } else {
                int jokeFd = open("/tmp/.joke", O_RDONLY);
                char buf;
                int rbytes;
                while ((rbytes = read(jokeFd, &buf, 1) > 0))
                    res += buf;
                res += "\n";
                close(jokeFd);
            }
        }
        message = ":" + __users[fd].getNickname() + " PRIVMSG " + __users[fd].getNickname() + " :" + res + "\n";
    } else if (command  == "/time") {
        std::time_t now = std::time(NULL);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        std::string current_time(buffer);
        current_time += "\n";
        message = ":" + __users[fd].getNickname() + " PRIVMSG " + __users[fd].getNickname() + " :" + current_time + "\n";
    }
    
    sendMessage(fd, message);
}