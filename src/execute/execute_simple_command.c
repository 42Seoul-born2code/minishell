#include "execute.h"

void	execute_simple_command(t_token *token_list, t_env_list env_list)
{
	pid_t			pid;
	int				outfile;
	char			*cmd_path;
	char			**cmd_argv;
	char			**envp;
	t_list			*curr_node;
	t_token_node	*curr_token;

	pid = fork();
	if (pid == CHILD_PROCESS)
	{
		curr_node = token_list->head_node;
		while (curr_node != NULL)
		{
			curr_token = curr_node->content;
			if (curr_token->type == COMMAND)
			{
				cmd_path = find_cmd_path(curr_token->word);
				if (cmd_path == NULL)
				{
					printf("%s: command not found\n", curr_token->word);
				}
				cmd_argv = merge_arguments(curr_node);
				envp = get_envp_in_list(&env_list);
			}
			else if (curr_token->type == REDIR_RIGHT)
			{
				outfile = open_file(curr_node->next, WRITE_MODE);
				dup2(outfile, STDOUT_FILENO);
			}
			curr_node = curr_node->next;
		}
		close(outfile);
		if (execve(cmd_path, cmd_argv, envp) == ERROR)
		{
			// printf("execve error occured\n");
		}
	}
}
