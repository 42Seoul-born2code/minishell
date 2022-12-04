
NAME 								:=	minishell

SRC_DIR							:=	./src/
INCLUDE							:=	-I./include/

SRC_EXECUTE_DIR			:=	execute/
SRC_EXECUTE					:=	execute_minishell.c

SRC_TOKENIZE_DIR		:=	tokenize/
SRC_TOKENIZE				:=	tokenize_line.c				\
												tokenize_utils.c

SRC_UTILS_DIR				:=	utils/
SRC_UTILS						:=	env_utils.c

SRC_FILES						:=	main.c																						\
												$(addprefix $(SRC_EXECUTE_DIR), $(SRC_EXECUTE)) 	\
												$(addprefix $(SRC_TOKENIZE_DIR), $(SRC_TOKENIZE)) \
												$(addprefix $(SRC_UTILS_DIR), $(SRC_UTILS)) 			\

SRCS								:=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS								:=	$(SRCS:%.c=%.o)

CC 									:=	cc
CFLAGS 							:=	-Wall -Wextra -Werror

LIB_NAME						:=	ft
LIB_DIR							:=	./libft/
LIB									:=	$(LIB_DIR)lib$(LIB_NAME).a
LIB_FLAGS						:=	-L $(LIB_DIR) -l$(LIB_NAME) -I$(LIB_DIR)

READLINE_LINKING		:=	-lreadline -L ~/.brew/opt/readline/lib
READLINE_COMPILE		:=	-I~/.brew/opt/readline/include

ifdef DEBUG_MODE
	CFLAGS						:=	$(CFLAGS) -g
endif

ifdef D_SANI
	CFLAGS						:=	$(CFLAGS) -g -fsanitize=address
endif

.PHONY : all
all : $(NAME)

.PHONY : clean
clean :
	rm -f $(OBJS)

.PHONY : fclean
fclean : clean
	rm -f $(NAME)

.PHONY : re
re : fclean all

debug : fclean
	make DEBUG_MODE=1 all

dsani : fclean
	make D_SANI=1 all

$(NAME) : $(OBJS)
	$(MAKE) -C $(LIB_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(READLINE_LINKING) $(LIB_FLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE) -I$(LIB_DIR) $(READLINE_COMPILE)