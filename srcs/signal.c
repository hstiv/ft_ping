#include "ft_ping.h"

void	signal_handler_alrm(int sig)
{
	g_ping->wait = false;
	sig = sig + (sig - sig);
}

void	signal_handler(int sig)
{
	print_statistics();
	sig = sig + (sig - sig);
	exit (EXIT_SUCCESS);
}
