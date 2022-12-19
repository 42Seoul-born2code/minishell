#include <unistd.h>
#include <stdio.h>

int main(void)
{
	char	*str;

	str = malloc(sizeof(char) * 10);
	if (str == NULL)
		exit(1);
	// malloc failed
	// str = NULL;
	printf("%c\n", str[0]);
}

// 1. 터지면 exit이 진행된다.
// 2. exit은 알아서 자원회수를 해주고 exit code 를 반환한다'.
