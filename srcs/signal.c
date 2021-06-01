#include "ft_ping.h"

// Handler for SIGINT, caused by
// Ctrl-C at keyboard
void			sigint_handler(int sig)
{
    printf("Caught signal %d\n", sig);
    // print_stat();
    exit (EXIT_SUCCESS);
}