#include "ft_ping.h"

void	signal_handler_alrm(__attribute__((unused))int sig)
{
	g_ping->wait = false;
}

void			signal_handler(__attribute__((unused))int sig)
{
    print_statistics();
    exit (EXIT_SUCCESS);
}