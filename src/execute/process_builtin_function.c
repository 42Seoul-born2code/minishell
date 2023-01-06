#include "minishell.h"
#include "execute.h"

t_bool	is_builtin_function(char *word)
{
	if (ft_strcmp(word, "echo") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "cd") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "pwd") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "export") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "unset") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "env") == 0)
		return (TRUE);
	else if (ft_strcmp(word, "exit") == 0)
		return (TRUE);
	return (FALSE);
}

t_bool	is_command_builtin_function(t_token *token_list)
{
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			if (is_builtin_function(curr_token->word) == TRUE)
				return (TRUE);
		}
		curr_node = curr_node->next;
	}
	return (FALSE);
}

void	process_builtin_function(t_token *token_list, t_env_list *env_list)
{
	t_redirect		redirect_info;
	int				origin_fd[2];
	char			*cmd_name;
	char			**cmd_argv;
	t_list			*curr_node;
	t_token_node	*curr_token;

	redirect_info.infile = NONE;
	redirect_info.outfile = NONE;
	redirect_info.type = NORMAL;
	redirect_info.heredoc_file_num = 0;
	curr_node = token_list->head_node;
	save_origin_fd(origin_fd);
	while (curr_node != NULL)
	{
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			cmd_name = ft_strdup(curr_token->word);
			cmd_argv = merge_arguments(curr_node);
		}
		else if (is_redirection(curr_token) == TRUE)
		{
			process_redirection(curr_node, &redirect_info);
		}
		curr_node = curr_node->next;
	}
	g_exit_code = execute_builtin_function(cmd_name, cmd_argv, env_list, SIMPLE_COMMAND);
	if (cmd_name != NULL)
		free(cmd_name);
	if (cmd_argv != NULL)
		free_all(cmd_argv);
	if (redirect_info.infile != NONE)
		close(redirect_info.infile);
	if (redirect_info.outfile != NONE)
		close(redirect_info.outfile);
	rollback_origin_fd(origin_fd);
	delete_heredoc_file(redirect_info.heredoc_file_num);
}
