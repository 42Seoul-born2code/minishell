#ifndef EXECUTE_H
# define EXECUTE_H

# include <stdio.h>
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

# define NONE	-1

# define ERROR_CODE_COMMAND_NOT_FOUND	127

# define NO_FILE_OR_DIR			"❌Error: No such file or directory: "
# define PERMISSON_DENIED		"❌Error: permission denied: "
# define PIPE_ERROR				"❌Error: pipe function doesn't work."
# define FORK_ERROR				"❌Error: fork error occured."
# define COMMAND_NOT_FOUND		"❌Error: command not found: "

# define HEREDOC_FILE			".heredoc.tmp"

typedef enum e_command_type{
	SIMPLE_COMMAND,
	MULTI_COMMAND,
}	t_command_type;

typedef enum e_pipe{
	READ,
	WRITE
}	t_pipe;

// execute_minishell.c
void	execute_minishell(t_env_list *env);
void	free_list_nodes(t_token *lst);

// execute_builtin_function.c
int		execute_builtin_function(char *cmd, char **argv, t_env_list *env_list);

// process_builtin_function.c
t_bool	is_command_builtin_function(t_token *token_list);
void	process_builtin_function(t_token *token_list, \
							t_env_list *env_list, t_command_type type);

// execute_command.c
void	execute_command(t_token *token_list, t_env_list *env_list);

// execute_multi_command.c
void	execute_multi_command(t_token *token_list, t_env_list *env_list);

// execute_simple_command.c
void	execute_simple_command(t_token *token_list, t_env_list *env_list);

// execute_utils.c
int		print_error(char *msg, char *arg);
t_bool	is_redirection(t_token_node *curr_token);
char	**merge_arguments(t_list *curr_node);
int		count_argv(t_list *curr_node);
char	*find_cmd_path(char *cmd);

// execute_utils2.c
t_bool	check_all_whitespace(char *line);

// fork_process.c
int		process_redirection(t_list *curr_node);
void	fork_process(t_token *token_list, t_env_list *env_list);

// open_file.c
int		open_file(t_list *next_node, int mode);


#endif