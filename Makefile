
NAME 					:=	minishell

SRC_FILES				:=	main.c
SRC_OBJS				:=	$(SRC_FILES:%.c=%.o)

CC 						:=	cc
CFLAGS 					:=	-Wall -Wextra -Werror
READLINE_LINKING		:=	-lreadline -L ~/.brew/opt/readline/lib
READLINE_COMPILE		:=	-I~/.brew/opt/readline/include

.PHONY : all clean fclean re

all : $(NAME)

clean :
	rm -f $(SRC_OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all

$(NAME) : $(SRC_OBJS)
	$(CC) $(CFLAGS) $(READLINE_LINKING) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@