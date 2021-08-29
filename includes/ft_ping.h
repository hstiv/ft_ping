#ifndef FT_PING_H
# define FT_PING_H

# include "libft.h"
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <signal.h>
# include <errno.h>
# include <stdbool.h>

# define ALLOWED_OPT "vh"
# define USAGE \
"Usage: \n\
  	ping [options] <destination> \n\
Options: \n\
  -v   -   verbose\n\
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

# define PROG_NAME	"ft_ping"
# define MAX_INET_ADDRSTRLEN (INET6_ADDRSTRLEN > INET_ADDRSTRLEN ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN)
# define PING_PACKET_SIZE	84
# define PING_PACKET_DATA_SIZE	(PING_PACKET_SIZE - sizeof(struct icmp) - sizeof(time_t))
# define ICMP_ECHO_CODE 0
# define RECV_TIMEOUT 1
# define TTL 64
# define INTERVAL 1

/*
**			options[OPT_V] = [v]
**			options[OPT_H] = [h]
*/

typedef	struct				s_pkt4
{
	// struct icmphdr			hdr;
	struct icmp				icmp;
	time_t					timestamp;
	char					msg[PING_PACKET_DATA_SIZE];
}							t_pkt4;

typedef struct				s_msg
{
	char					buff[PING_PACKET_SIZE];
	struct msghdr			hdr;
	struct iovec			iov;
	ssize_t					read_bytes;
	struct icmp				*icmp4;
	void					*ctrl;
}							t_msg;

typedef	struct				s_loop
{
	struct	timeval			tv;
	t_msg					msg;
	t_pkt4					pckt;
	struct	sockaddr_in		r_addr;
	int						addr_len;
	int						msg_count;
	int						send_allowed;
}							t_loop;

typedef struct				s_ping_stat {
	uint16_t				icmp_send;
	uint16_t				icmp_rcv;
	uint16_t				icmp_error;
	struct timeval			tv_ping_start;
	struct timeval			tv_ping_end;
	double					rtt_square_sum;
	double					rtt_sum;
	double					rtt_min;
	double					rtt_max;
}							t_ping_stat;

typedef	struct 				s_ping
{
	int						options[3];
	char					*hostname;
	struct	timeval			tv_start;
	struct	timeval			tv_end;
	struct	addrinfo		*addr_res;
	int						sockfd;
	char					ip_addr[MAX_INET_ADDRSTRLEN];
	struct	sockaddr_in		ip4;
	char					*reversed_dns;
	t_ping_stat				stat;
	int						ttl;
	bool					wait;
}							t_ping;

extern	t_ping				*g_ping;

int							get_host_ip();
void						signal_handler_alrm(__attribute__((unused))int sig);
void						signal_handler(__attribute__((unused))int sig);
void						loop(void);
void						fill_send_packet(t_loop *state);
void						fill_recv_msg(t_msg *msg_recv);
void						read_ping(t_msg *reply);
bool						inspect_and_print_ping(t_msg *msg_recv, struct timeval *tv);
void						print_ping_gen(struct timeval *tv_seq_start, ssize_t read_icmp_bytes, uint16_t rcv_icmp_seq);
void						print_statistics(void);

#endif