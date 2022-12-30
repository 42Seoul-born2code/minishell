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

void	save_origin_fd(int origin_fd[2])
{
	origin_fd[0] = dup(STDIN_FILENO);
	origin_fd[1] = dup(STDOUT_FILENO);
}

void	rollback_origin_fd(int origin_fd[2])
{
	dup2(origin_fd[0], STDIN_FILENO);
	dup2(origin_fd[1], STDOUT_FILENO);
	close(origin_fd[0]);
	close(origin_fd[1]);
}

void	process_builtin_function(t_token *token_list, t_env_list *env_list, t_command_type type)
{
	int				file;
	int				origin_fd[2];
	char			*cmd;
	char			**cmd_argv;
	t_list			*curr_node;
	t_token_node	*curr_token;

	curr_node = token_list->head_node;
	while (curr_node != NULL)
	{
		file = NONE;
		curr_token = curr_node->content;
		if (curr_token->type == COMMAND)
		{
			cmd = ft_strdup(curr_token->word);
			cmd_argv = merge_arguments(curr_node);
		}
		else if (is_redirection(curr_token) == TRUE)
		{
			save_origin_fd(origin_fd);
			file = process_redirection(curr_node);
		}
		curr_node = curr_node->next;
	}
	if (type == SIMPLE_COMMAND)
	{
		execute_builtin_function(cmd, cmd_argv, env_list);
		if (file != NONE)
			close(file);
		rollback_origin_fd(origin_fd);
	}
	else
	{

	}
}
