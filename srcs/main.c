#include "ft_ping.h"

void	ft_perror(char *s, int code)
{
	if (code == 0)
		dprintf(STDERR_FILENO, "ft_pint: %s\n", s);
	else
		perror(s);
	if (g_ping->reversed_dns)
		free(g_ping->reversed_dns);
	if (g_ping->sockfd != -1)
		close(g_ping->sockfd);
	exit(EXIT_FAILURE);
}

static int	set_option(t_ping *ping, char c)
{
	if (ft_strchr(VALID_OPTIONS, c) == NULL)
		return (EXIT_FAILURE);
	if (c == 'v')
		ping->options[OPT_V] = 1;
	if (c == 'h')
		ping->options[OPT_H] = 1;
	return (EXIT_SUCCESS);
}

static int	parse_options(int argc, char **argv, t_ping *ping)
{
	int				i;
	int				j;

	i = OPT_SIZE;
	while (--i >= 0)
		ping->options[i] = 0;
	i = 0;
	while (++i < argc)
	{
		if (argv[i][0] == '-')
		{
			j = 0;
			while (argv[i][++j] != '\0')
				if (set_option(ping, argv[i][j]) == EXIT_FAILURE)
					return (EXIT_FAILURE);
		}
		else
			ping->hostname = ft_strdup(argv[i]);
	}
	return (EXIT_SUCCESS);
}

static void	init_params(t_ping *data)
{
	g_ping = data;
	ft_bzero(&data->stat, sizeof(data->stat));
	g_ping->ttl = TTL;
	if (get_host_ip() == EXIT_FAILURE)
		exit (EXIT_FAILURE);
}

int	main(int argc, char	**argv)
{
	t_ping			data;

	if (argc <= 1 || parse_options(argc, argv, &data) == EXIT_FAILURE)
	{
		dprintf(STDERR_FILENO, "%s\n", USAGE);
		exit (EXIT_FAILURE);
	}
	if (data.options[OPT_H])
	{
		dprintf(STDERR_FILENO, "%s\n", USAGE);
		exit (EXIT_SUCCESS);
	}
	init_params(&data);
	signal(SIGINT, &signal_handler);
	signal(SIGALRM, &signal_handler_alrm);
	loop();
	exit (EXIT_SUCCESS);
}
