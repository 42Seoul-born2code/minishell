/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joonhan <joonhan@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:18:56 by joonhan           #+#    #+#             */
/*   Updated: 2023/01/08 13:11:13 by joonhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include "mini_signal.h"

# define PROMPT 		"./minishell$ "
# define HEREDOC_PROMPT "heredoc> "

# define CHILD_PROCESS	0
# define PARENT_PROCESS	1

# define WRITE_MODE		0
# define READ_MODE		1
# define APPEND_MODE	2

# define NONE	-1

# define ERROR_CODE_COMMAND_NOT_FOUND	127

# define NO_FILE_OR_DIR			"❌Error: No such file or directory: "
# define PERMISSION_DENIED		"❌Error: permission denied: "
# define PIPE_ERROR				"❌Error: pipe function doesn't work."
# define FORK_ERROR				"❌Error: fork error occured."
# define COMMAND_NOT_FOUND		"❌Error: command not found: "

# define HEREDOC_FILE			"/tmp/.heredoc.tmp"

typedef enum e_command_type{
	SIMPLE_COMMAND,
	MULTI_COMMAND,
}	t_command_type;

typedef enum e_pipe{
	READ,
	WRITE,
}	t_pipe;

typedef enum e_redir_type{
	NORMAL,
	INFILE,
	OUTFILE,
	HEREDOC
}	t_redir_type;

typedef enum e_init{
	INIT,
	FREE
}	t_init;

typedef struct s_redirect{
	int				infile;
	int				outfile;
	int				heredoc_file_num;
	t_redir_type	type;
}	t_redirect;

typedef struct s_cmd_info{
	char	*cmd_name;
	char	**cmd_argv;
}	t_cmd_info;

// execute_minishell.c
void	execute_minishell(t_token *token_list, t_env_list *env);

// execute_builtin_function.c
int		execute_builtin_function(char *cmd, char **argv, \
					t_env_list *env_list, t_command_type command_type);

// process_builtin_function.c
t_bool	is_builtin_function(char *word);
t_bool	is_command_builtin_function(t_token *token_list);
void	process_builtin_function(t_token *token_list, t_env_list *env_list);

// execute_command.c
void	execute_command(t_token *token_list, t_env_list *env_list);

// execute_cmd.c
void	execute_cmd(char *cmd_path, char **cmd_argv, \
					t_redirect redirect_info, t_env_list *env_list);

// execute_multi_command.c
void	execute_multi_command(t_token *token_list, t_env_list *env_list);

// execute_utils.c
t_bool	is_redirection(t_token_node *curr_token);
char	**merge_arguments(t_list *curr_node);
int		count_argv(t_list *curr_node);
void	close_pipes(int pipe_fd[2]);

// execute_utils2.c
void	init_cmd_info(t_cmd_info *cmd_info, t_init type);
void	init_redirect_info(t_redirect *redirect_info);
int		process_tokens(t_list *curr_node, t_cmd_info *cmd_info, \
				t_redirect *redirect_info, t_env_list *env_list);

// find_cmd_path.c
char	*find_cmd_path(char *cmd, t_env_list *env_list);

// fork_process.c
void	fork_process(t_token *token_list, t_env_list *env_list);
char	*make_heredoc_file_name(int idx);

// open_file.c
int		open_file(t_list *next_node, int mode);

// redirection_utils.c
void	save_origin_fd(int origin_fd[2]);
void	rollback_origin_fd(int origin_fd[2]);

// redirection_utils2.c
void	no_redirect(t_redirect *redirect_info);
void	redirect_right(t_list *curr_node, t_redirect *redirect_info);
void	redirect_heredoc(t_redirect *redirect_info);
void	redirect_append(t_list *curr_node, t_redirect *redirect_info);
void	redirect_left(t_list *curr_node, t_redirect *redirect_info);

// heredoc_utils.c
int		get_heredoc_file_fd(int heredoc_idx, int mode);
char	*merge_word_list(t_word_list *word_list);
void	delete_heredoc_file(int heredoc_file_num);

// check_heredoc.c
void	check_heredoc(t_token *token_list);
void	get_user_input(char *limiter, int idx);

// expand_env_variable.c
char	*expand_env_variable(char *input);

// process_redirection.c
void	process_redirection(t_list *curr_node, t_redirect *redirect_info);
#endif