#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/icmp6.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <signal.h>

# define ALLOWED_OPT "vh"
# define USAGE \
"Usage: \n\
  	ping [options] <destination> \n\
Options: \n\
  -a   -   use audible ping \n\
  -v \n\
  -h   -   help"

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

# define MAX_INET_ADDRSTRLEN (INET6_ADDRSTRLEN > INET_ADDRSTRLEN ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN)
# define PING_PACKET_SIZE	84
# define PING_PACKET_DATA_SIZE	(PING_PACKET_SIZE - sizeof(struct icmp) - sizeof(time_t))

/*
**			options[OPT_V] = [v]
**			options[OPT_H] = [h]
*/

typedef	struct				s_pkt4 {
	struct icmp				icmp;
	time_t					timestamp;
	char					data[PING_PACKET_DATA_SIZE];
}							t_pkt4;

typedef	struct				s_pkt6 {
	struct icmp6_hdr		icmp;
	time_t					timestamp;
	char					data[PING_PKT_DATA_SIZE];
}							t_pkt6;


typedef	struct 				s_ping
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
	char					ip_addr[MAX_INET_ADDRSTRLEN];
	struct	sockaddr_in		ip4;
	char					*reversed_dns;
}							t_ping;

extern	t_ping				*g_ping;

int							get_host_ip(t_ping  *data);
void						sigint_handler(int sig);
void						loop(void);

void						fill_pkt4(struct s_pkt4 *pkt, time_t timestamp);

#endif