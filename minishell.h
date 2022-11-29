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

typedef struct	s_envp {
	int		node_count;
	t_list	*head_node;
}	t_envp;

# define INVALID_ARGUMENT	"❌Error: Invalid arguments"

#endif