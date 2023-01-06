#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

void	classify(struct dirent *ent)
{
	printf("%s\t", ent->d_name);
	if (ent->d_type == DT_BLK)
		printf("Block Device\n");
	else if (ent->d_type == DT_CHR)
		printf("Character Device\n");
	else if (ent->d_type == DT_DIR)
		printf("Directory\n");
	else if (ent->d_type == DT_LNK)
		printf("Symbolic Link\n");
	else if (ent->d_type == DT_REG)
		printf("Regular File\n");
	else if (ent->d_type == DT_SOCK)
		printf("Unix Domain Socket\n");
	else
		printf("Unknown Type File\n");
}

int	main(void)
{
	DIR				*dirp;
	struct dirent	*file;

    dirp = opendir("test_dir");
	if (dirp == NULL)
		printf("NULLLLLLLL");
	// while (42)
	// {
	// 	file = readdir(dirp);
	// 	if (!file)
	// 		break ;
	// 	classify(file);
	// }
	// closedir(dirp);
}