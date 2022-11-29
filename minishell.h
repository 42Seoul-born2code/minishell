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
# define FALSE	0
# define TRUE	1
typedef struct	s_env_list {
	t_list	*head_node;
}	t_env_list;

typedef struct	s_env_node {
	char	*line;
	char	*key;
	char	*value;
}	t_env_node;

# define INVALID_ARGUMENT	"❌Error: Invalid arguments"

#endif