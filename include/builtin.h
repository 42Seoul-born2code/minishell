#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <fcntl.h>

# include "tokenize.h"
# include "parsing.h"
# include "expansion.h"
# include "utils.h"

// check_option.c
int	check_option(char *argv);

// ft_echo.c
int	ft_echo(char **argv);

// ft_pwd.c
int ft_pwd(char **argv);

#endif