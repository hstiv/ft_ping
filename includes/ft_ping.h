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

# define TEST \
"-----------------------------------------\n\
|	ai_flags        =	%d	|\n\
|	ai_family       =	%d	|\n\
|	ai_socktype     =	%d	|\n\
|	ai_protocol     =	%d	|\n\
|	ai_addrlen      =	%d	|\n\
|	ai_canonname    =	%s	|\n\
------------------------------------------\n\n"

# define OPT_V 0
# define OPT_H 1
# define OPT_SIZE 2

/*
**			options[OPT_V] = [v]
**			options[OPT_H] = [h]
*/

typedef struct 				s_ping
{
	int						options[3];
	char					*hostname;
	pid_t					pid;
	uid_t					uid;
	struct	timeval			tv;
	struct	timezone		tz;
	struct	addrinfo		*addr_res;
	int						ai_family;
	struct	msghdr			*msg;
	int						sockfd;
	char					ip_addr[INET_ADDRSTRLEN];
	struct	sockaddr_in		ip4;
	char					*reverse_dns;
}							t_ping;

int							get_host_ip(t_ping  *data);

#endif