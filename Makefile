
NAME 						:=	minishell

SRC_DIR						:=	./src/
INCLUDE						:=	-I./include/

SRC_BUILTIN_DIR				:=	builtin/
SRC_BUILTIN					:=	execute_builtin_function.c		\
								ft_cd.c							\
								ft_echo.c						\
								ft_pwd.c						\
								ft_unset.c						\
								ft_env.c						\
								ft_export.c						\
								builtin_utils.c					\
								check_option.c

SRC_EXECUTE_DIR				:=	execute/
SRC_EXECUTE					:=	execute_minishell.c				\
								process_builtin_function.c		\
								execute_command.c				\
								execute_simple_command.c		\
								execute_multi_command.c			\
								execute_utils.c					\
								fork_process.c					\
								open_file.c				


SRC_TOKENIZE_DIR			:=	tokenize/
SRC_TOKENIZE				:=	tokenize_line.c			\
								tokenize_utils.c

SRC_PARSING_DIR				:=	parsing/
SRC_PARSING					:=	parsing.c				\
								syntax_analysis.c

SRC_EXPANSION_DIR			:=	expansion/
SRC_EXPANSION				:=	expansion.c				\
								quote_removal.c

SRC_REDIRECT_DIR			:=	redirect/
SRC_REDIRECT				:=	redirect.c

SRC_UTILS_DIR				:=	utils/
SRC_UTILS					:=	env_utils.c				\
								error.c


SRC_FILES					:=	main.c												\
								$(addprefix $(SRC_BUILTIN_DIR), $(SRC_BUILTIN)) 	\
								$(addprefix $(SRC_EXECUTE_DIR), $(SRC_EXECUTE)) 	\
								$(addprefix $(SRC_TOKENIZE_DIR), $(SRC_TOKENIZE)) 	\
								$(addprefix $(SRC_PARSING_DIR), $(SRC_PARSING)) 	\
								$(addprefix $(SRC_EXPANSION_DIR), $(SRC_EXPANSION)) \
								$(addprefix $(SRC_UTILS_DIR), $(SRC_UTILS)) 		\
								$(addprefix $(SRC_BUILTIN_DIR), $(SRC_BUILTIN)) 

SRCS						:=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS						:=	$(SRCS:%.c=%.o)

CC 							:=	cc
CFLAGS 						:=	-Wall -Wextra -Werror

LIB_NAME					:=	ft
LIB_DIR						:=	./libft/
LIB							:=	$(LIB_DIR)lib$(LIB_NAME).a
LIB_FLAGS					:=	-L $(LIB_DIR) -l$(LIB_NAME) -I$(LIB_DIR)

ifdef HOME
	READLINE_LINKING			:=	-lreadline -L/usr/local/opt/readline/lib
	READLINE_COMPILE			:=	-I/usr/local/opt/readline/include
else
	READLINE_LINKING			:=	-lreadline -L ${HOME}/.brew/opt/readline/lib
	READLINE_COMPILE			:=	-I${HOME}/.brew/opt/readline/include
endif

ifdef DEBUG_MODE
	CFLAGS					:=	$(CFLAGS) -g
endif

ifdef D_SANI
	CFLAGS					:=	$(CFLAGS) -g -fsanitize=address
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

debugh : fclean
	make -j4 DEBUG_MODE=1 HOME=1 all

dsanih : fclean
	make -j4 D_SANI=1 HOME=1 all

debug : fclean
	make -j4 DEBUG_MODE=1 all

dsani : fclean
	make -j4 D_SANI=1 all

$(NAME) : $(OBJS)
	$(MAKE) -j4 -C $(LIB_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(READLINE_LINKING) $(LIB_FLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE) -I$(LIB_DIR) $(READLINE_COMPILE)