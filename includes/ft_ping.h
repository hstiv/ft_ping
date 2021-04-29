#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <signal.h>

# define ALLOWED_OPT "vh"
# define USAGE \
"Usage: \
  	ping [options] <destination> \
Options: \
  <destination>      dns name or ip address \
  -a                 use audible ping \
  -v \
  -h								"

typedef struct 			s_ping
{
	pid_t				pid;
	uid_t				uid;
	struct	timeval		tv;
	struct	timezone	tz;
	struct	addinfo		*res;
	struct	msghdr		*msg;
	int					sockfd;
	char				ipv4_addr[sizeof(struct in_addr)];
}						t_ping;

#endif