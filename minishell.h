#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include "./libft/libft.h"

# define ERROR	-1

# define INVALID_ARGUMENT	"❌Error: Invalid arguments"

enum e_meta {
	WHITESPACE,
	WORD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	DOLLAR,
	EQUAL,
	BACKSLASH,
	PIPE,
	REDIR_LEFT,
	REDIR_HEREDOC,
	REDIR_RIGHT,
	REDIR_APPEND
};

typedef struct s_env_list {
	t_list	*head_node;
}	t_env_list;

typedef struct s_env_node {
	char	*line;
	char	*key;
	char	*value;
}	t_env_node;

typedef struct s_token {
	t_list	*head_node;
}	t_token;

typedef struct s_token_node {
	int		type;
	char	*word;
}	t_token_node;

// env_utils.c
char	**get_envp_in_list(t_env_list *curr_env);
void	save_envp_in_list(char **envp, t_env_list *curr_env);

// execute_minishell.c
void	execute_minishell(t_env_list env);

#endif