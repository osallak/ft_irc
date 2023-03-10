
// # include "utils.hpp"
# include "Server.hpp"


int main(int ac, char **av)
{
    Server server;

    if (ac != 3)
    {
        std::cout << "Usage: ./irc [port] [password]" << std::endl;
        return (1);
    }
    try{
        server.setPassword(av[2]);//check if password is valid
        server.setPort(av[1]);//check if port is valid
        server.run();
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
