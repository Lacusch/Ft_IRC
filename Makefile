NAME		= ircserv
	
CC			= c++
RM			= rm -rf
FLAGS		= -Wall -Wextra -Werror -std=c++98

OBJDIR 		= .obj
FILES		= main
SRC			= $(FILES:=.cpp)
OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADERS	= 
 
RED			= 	\e[31m
GREEN		=	\e[92;5;118m
RESET		=	\e[0m
CURSIVE		=	\e[33;3m

.PHONY: all clean fclean re bonus norm

all: $(NAME)

$(NAME): $(OBJ) $(HEADERS)
	@$(CC) $(OBJ) $(OPTS) -o $(NAME)
	@printf "$(CURSIVE)$(GREEN)- Compiling ...\n$(RESET)"

$(OBJDIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) $(OPTS) -c $< -o $@

clean:
	@$(RM) $(OBJDIR) $(OBJ)
	@printf "$(CURSIVE)$(RED)- Object files removed.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(CURSIVE)$(RED)- Executable removed.$(RESET)\n"

re: fclean all