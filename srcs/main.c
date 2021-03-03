#include "ft_ping.h"

int				main(int argc, char	**argv)
{
	int			i;

	i = 1;
	printf("It is the very beginning %d:\n", argc);
	while (i < argc)
		printf("*-> %s\n", argv[i]);
	return (0);
}