#include "execute.h"

static int	open_read_mode_file(char *file)
{
	if (access(file, F_OK) == ERROR)
		print_error(NO_FILE_OR_DIR, file);
	else if (access(file, R_OK) == ERROR)
		print_error(PERMISSON_DENIED, file);
	return (open(file, O_RDONLY));
}

static int	open_write_mode_file(char *file, int mode)
{
	int	fd;

	if (mode == WRITE_MODE)
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else
		fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == ERROR)
	{
		if (access(file, F_OK) == ERROR)
			print_error(NO_FILE_OR_DIR, file);
		else if (access(file, W_OK | X_OK) == ERROR)
			print_error(PERMISSON_DENIED, file);
	}
	return (fd);
}

int	open_file(t_list *next_node, int mode)
{
	char	*file;

	file = ((t_token_node *)next_node->content)->word;
	if (mode == READ_MODE)
	{
		return (open_read_mode_file(file));
	}
	else if (mode == WRITE_MODE || mode == APPEND_MODE)
	{
		return (open_write_mode_file(file, mode));
	}
	return (ERROR);
}
