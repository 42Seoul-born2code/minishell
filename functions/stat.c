#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/stat.h>

/*

File type:                regular file
I-node number:            421032
Mode:                     100664 (octal)
Link count:               1
Ownership:                UID=1000   GID=1000
Preferred I/O block size: 4096 bytes
File size:                1611 bytes
Blocks allocated:         8
Last status change:       Sat Nov  5 18:07:33 2016
Last file access:         Sat Nov  5 18:07:38 2016
Last file modification:   Sat Nov  5 18:07:33 2016

*/

int	main(void)
{
	char	*input;
	mode_t	file_mode;
    struct stat buf;
    
	while (42)
	{
		// Display prompt and read input (n.b. input must be freed after use)...
		input = readline("minishell$ ");

		// Check for EOF.
		if (!input)
			break;

		// Add input to history.
		add_history(input);

		stat(input, &buf);
		file_mode = buf.st_mode;
		if (S_ISREG(file_mode))
		{
			printf("Regular file\n");
		}
		else if (S_ISDIR(file_mode))
		{
			printf("Directory\n");
		}

		// Free input.
		free(input);
	}
}