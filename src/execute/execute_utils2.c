#include "execute.h"
#include "tokenize.h"

t_bool	check_all_whitespace(char *line)
{
	int i;
	t_bool all_whitespace;

	i = 0;
	all_whitespace = TRUE;
	while (line[i] != '\0')
	{
		if (is_whitespace(line[i]) == FALSE)
			all_whitespace = FALSE;
		return all_whitespace;
		i += 1;
	}
	return all_whitespace;
}