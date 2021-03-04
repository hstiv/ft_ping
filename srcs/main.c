#include "ft_ping.h"

int				main(int argc, char	**argv)
{
	int			i;

	i = 1;
	while (i < argc)
		printf("*-> %s\n", argv[i]);
	exit (EXIT_SUCCESS);
}