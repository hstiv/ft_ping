#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <signal.h>

# define ALLOWED_OPT "vh"

typedef struct 			s_ping
{
	pid_t				pid;
	uid_t				uif;
	struct	timeval		*tv;
	struct	timezone	*tz;
	struct	addrinfo	*res;
	struct	msghdr		*msg;
	
}						t_ping;

#endif