NAME = bin/ircserv

SRC =  Server.cpp main.cpp  Client.cpp Channel.cpp utils.cpp
CC = c++
CFLAGS = -Wall -Wextra -Werror -c  -Iinclude -fsanitize=address -g
LFLAGS = -fsanitize=address

BLUE = \033[0;34m
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
CYAN = \033[0;36m
NC = \033[0m

SRCS = $(addprefix src/, $(SRC))
OBJS = $(addprefix obj/, $(SRC:.cpp=.o))
OBJDIR = obj
SRCDIR = src

INC = Server.hpp Client.hpp Channel.hpp

INCS = $(addprefix src/, $(INC)) include/utils.hpp

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCS)
	@echo "[$(BLUE) OK $(NC)] $(YELLOW)Compiling $<$(NC)"
	@$(CC) $(CFLAGS) $< -o $@

$(NAME) : $(OBJS) 
	@$(CC) $^ $(LFLAGS) -o $@
	@echo "\n ✅ $(GREEN)$(NAME) created$(NC)"

clean :
	@/bin/rm -rf $(OBJS)
	@echo "$(RED)Objects removed$(NC)"

fclean: clean
	@/bin/rm -rf $(NAME)
	@echo "$(RED)$(NAME) removed$(NC)"


re : fclean $(NAME)