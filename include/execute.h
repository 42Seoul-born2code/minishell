#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "tokenize.h"
# include "parsing.h"
# include "expansion.h"
# include "utils.h"

# define PROMPT "./minishell$ "

# define CHILD_PROCESS	0

# define WRITE_MODE		0
# define READ_MODE		1
# define APPEND_MODE	2

# define NO_FILE_OR_DIR			"❌Error: No such file or directory: "
# define PERMISSON_DENIED		"❌Error: permission denied: "

typedef enum {
	SIMPLE_COMMAND,
	MULTI_COMMAND,
}	e_command_type;

// execute_minishell.c
void	execute_minishell(t_env_list env);

// execute_command.c
void	execute_command(t_token *token_list, t_env_list env_list);

// execute_simple_command.c
void	execute_simple_command(t_token *token_list, t_env_list env_list);

// execute_utils.c
void	print_error(char *msg, char *arg);
t_bool	is_redirection(t_token_node *curr_token);
char	**merge_arguments(t_list *curr_node);
int	    count_argv(t_list *curr_node);
char	*find_cmd_path(char *cmd);

// open_file.c
int	    open_file(t_list *next_node, int mode);

#endif