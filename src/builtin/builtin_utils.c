#include "libft.h"

t_bool	is_valid_variable_name(char *str)
{
	int	idx;

	if (ft_isdigit(str[0]) == TRUE)
		return (FALSE);
	idx = 0;
	while (str[idx] != '\0')
	{
		if (ft_isalnum(str[idx]) == TRUE || str[idx] == '_')
			idx += 1;
		else
			return (FALSE);
	}
	return (TRUE);
}
