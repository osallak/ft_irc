# ft_irc

ft_irc is a small-scale IRC server implemented in C++ as part of the 42 School curriculum. The server uses the TCP protocol to communicate with clients and supports basic features such as user authentication, channel creation, and message broadcasting.
Installation

To install and run ft_irc on your local machine, follow these steps:

    Clone the repository onto your machine using git clone https://github.com/osallak/ft_irc.git
    Navigate into the repository directory using cd ft_irc
    Run the make command to compile the project using make
    Start the server using the command ./ircserv <port> <password>

```Usage```

Once the server is running, you can connect to it using an IRC client such as HexChat or mIRC. Connect to the server using the hostname and port number specified when starting the server.

You can use the following commands to interact with the server:

    NICK: Changes the user's nickname
    USER: Sets the user's username and realname
    JOIN: Joins a channel
    PART: Leaves a channel
    TOPIC: Sets the topic of a channel
    NAMES: Lists the users in a channel
    LIST: Lists all the available channels
    PRIVMSG: Sends a message to a user or a channel 
    NOTICE: Sends a notice to a user or a channel
    PING: Pings the server to check the connection
    QUIT: Disconnects from the server
    /TIME: display the currnet time
    /JOKE: dad jokes
    

For a full list of commands supported by the server, see the IRC RFC.
Contributing

If you'd like to contribute to ft_irc, feel free to submit a pull request or create an issue on the repository. We welcome any suggestions or improvements to the project.
License

This project is licensed under the MIT license. Feel free to use and modify the code as needed.
