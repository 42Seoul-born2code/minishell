# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeongkpa <jeongkpa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/06 16:19:07 by joonhan           #+#    #+#              #
#    Updated: 2023/01/08 15:59:23 by jeongkpa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 						:=	minishell

SRC_DIR						:=	./src/
INCLUDE						:=	-I./include/

SRC_BUILTIN_DIR				:=	builtin/
SRC_BUILTIN					:=	execute_builtin_function.c			\
								ft_cd.c								\
								ft_cd_utils.c						\
								ft_echo.c							\
								ft_pwd.c							\
								ft_unset.c							\
								ft_env.c							\
								ft_export.c							\
								ft_exit.c							\
								builtin_utils.c					

SRC_EXECUTE_DIR				:=	execute/
SRC_EXECUTE					:=	execute_minishell.c					\
								process_builtin_function.c			\
								execute_command.c					\
								execute_cmd.c						\
								execute_multi_command.c				\
								execute_utils.c						\
								execute_utils2.c					\
								find_cmd_path.c						\
								fork_process.c						\
								open_file.c							\
								heredoc_utils.c						\
								check_heredoc.c						\
								expand_env_variable.c				\
								process_redirection.c				\
								redirection_utils.c					\
								redirection_utils2.c


SRC_TOKENIZE_DIR			:=	tokenize/
SRC_TOKENIZE				:=	tokenize_line.c						\
								tokenize_utils.c

SRC_PARSING_DIR				:=	parsing/
SRC_PARSING					:=	parsing.c							\
								syntax_analysis.c

SRC_SIGNAL_DIR				:=	signal/
SRC_SIGNAL					:=	init_signal.c						\
								change_signal.c						\
								heredoc_signal.c					\
								termios.c

SRC_EXPANSION_DIR			:=	expansion/
SRC_EXPANSION				:=	expansion.c							\
								expansion_utils.c					\
								quote_removal.c						\
								remove_whitespace.c					\
								save_expand_env_variable.c			\
								save_word_and_expanded_variable.c

SRC_REDIRECT_DIR			:=	redirect/
SRC_REDIRECT				:=	redirect.c

SRC_UTILS_DIR				:=	utils/
SRC_UTILS					:=	env_utils.c							\
								error.c


SRC_FILES					:=	main.c												\
								$(addprefix $(SRC_BUILTIN_DIR), $(SRC_BUILTIN)) 	\
								$(addprefix $(SRC_EXECUTE_DIR), $(SRC_EXECUTE)) 	\
								$(addprefix $(SRC_TOKENIZE_DIR), $(SRC_TOKENIZE)) 	\
								$(addprefix $(SRC_PARSING_DIR), $(SRC_PARSING)) 	\
								$(addprefix $(SRC_SIGNAL_DIR), $(SRC_SIGNAL)) 		\
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

ifdef HOME_ENV
	READLINE_LINKING			:=	-lreadline -L/usr/local/opt/readline/lib
	READLINE_COMPILE			:=	-I/usr/local/opt/readline/include
else
	READLINE_LINKING			:=	-lreadline -L ${HOME}/.brew/opt/readline/lib
	READLINE_COMPILE			:=	-I${HOME}/.brew/opt/readline/include
endif

### For M1 Mac Mini ###
ifdef MAC_MINI
	READLINE_LINKING			:=	-lreadline -L/opt/homebrew/opt/readline/lib
	READLINE_COMPILE			:=	-I/opt/homebrew/opt/readline/include
endif
### For M1 Mac Mini ###

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
#	$(MAKE) clean -C $(LIB_DIR)

.PHONY : fclean
fclean : clean
	rm -f $(NAME)
#	$(MAKE) fclean -C $(LIB_DIR)

.PHONY : re
re : fclean all

debugh : fclean
	make -j4 DEBUG_MODE=1 HOME_ENV=1 all

dsanih : fclean
	make -j4 D_SANI=1 HOME_ENV=1 all

debugm : fclean
	make -j4 DEBUG_MODE=1 MAC_MINI=1 all

dsanim : fclean
	make -j4 D_SANI=1 MAC_MINI=1 all

debug : fclean
	make -j4 DEBUG_MODE=1 all

dsani : fclean
	make -j4 D_SANI=1 all

$(NAME) : $(OBJS)
	$(MAKE) -j4 -C $(LIB_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(READLINE_LINKING) $(LIB_FLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE) -I$(LIB_DIR) $(READLINE_COMPILE)




