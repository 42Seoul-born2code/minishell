
NAME 					:=	minishell

SRC_FILES				:=	main.c				\
							env_utils.c
SRC_OBJS				:=	$(SRC_FILES:%.c=%.o)

CC 						:=	cc
CFLAGS 					:=	-Wall -Wextra -Werror

LIB_NAME				:=	ft
LIB_DIR					:=	./libft/
LIB						:=	$(LIB_DIR)lib$(LIB_NAME).a
LIB_FLAGS				:=	-L $(LIB_DIR) -l$(LIB_NAME)

READLINE_LINKING		:=	-lreadline -L ~/.brew/opt/readline/lib
READLINE_COMPILE		:=	-I~/.brew/opt/readline/include

ifdef DEBUG_MODE
	CFLAGS				:=	$(CFLAGS) -g
endif

.PHONY : all clean fclean re

all : $(NAME)

clean :
	rm -f $(SRC_OBJS)

debug : fclean
	make DEBUG_MODE=1 all

fclean : clean
	rm -f $(NAME)

re : fclean all

$(NAME) : $(SRC_OBJS)
	$(MAKE) -C $(LIB_DIR)
	$(CC) $(CFLAGS) $(READLINE_LINKING) $(LIB_FLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) $(READLINE_COMPILE) -c $< -o $@