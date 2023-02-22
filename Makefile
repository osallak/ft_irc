SRC = src/main.cpp src/Server.cpp src/Client.cpp src/Channel.cpp

OBJ = $(SRC:.cpp=.o)

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

CC = c++

NAME = IRC

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $^  -o $@

%.o: %.cpp src/Server.hpp src/Client.hpp src/Channel.hpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all