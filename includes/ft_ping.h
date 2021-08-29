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
# include <math.h>

# define ALLOWED_OPT "vh"
# define USAGE "Usage: \n\tping [options] <destination> \n\
Options: \n\t-v\t-\tverbose\n\t-h\t-\thelp"

# define OPT_V 0
# define OPT_H 1
# define OPT_SIZE 2

# define PROG_NAME	"ft_ping"
# define MAX_INET_ADDRSTRLEN INET_ADDRSTRLEN
# define PING_PACKET_SIZE	84
# define PING_PACKET_DATA_SIZE	48
# define ICMP_ECHO_CODE 0
# define RECV_TIMEOUT 1
# define TTL 64
# define INTERVAL 1

# define GETTIMEOFDAY_ERR "ERROR:[gettimeofday]: One of \
[tv] or [tz] pointed outside the accessible address space.\n"
# define PRINT_STAT_ICMP_ERR "%hu packets transmitted, %hu \
received, +%hu errors, %.0lf%% packet loss, time %.0lfms\n"
# define PRINT_STAT "%hu packets transmitted, %hu received, \
%.0lf%% packet loss, time %.0lfms\n"
# define PRINT_STAT2 "rtt min/avg/max/mdev = %.3lf/%.3lf/%.3lf/%.3lf ms\n"

/*
**			options[OPT_V] = [v]
**			options[OPT_H] = [h]
*/

typedef struct s_pkt4
{
	struct icmp				icmp;
	time_t					timestamp;
	char					msg[PING_PACKET_DATA_SIZE];
}							t_pkt4;

typedef struct s_msg
{
	char					buff[PING_PACKET_SIZE];
	struct msghdr			hdr;
	struct iovec			iov;
	ssize_t					read_bytes;
	struct icmp				*icmp4;
	void					*ctrl;
}							t_msg;

typedef struct s_loop
{
	struct timeval			tv;
	t_msg					msg;
	t_pkt4					pckt;
	struct sockaddr_in		r_addr;
	int						addr_len;
	int						send_allowed;
}							t_loop;

typedef struct s_ping_stat {
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

typedef struct s_ping
{
	int						options[3];
	char					*hostname;
	struct timeval			tv_start;
	struct timeval			tv_end;
	struct addrinfo			*addr_res;
	int						sockfd;
	char					ip_addr[MAX_INET_ADDRSTRLEN];
	struct sockaddr_in		ip4;
	char					*reversed_dns;
	t_ping_stat				stat;
	int						ttl;
	bool					wait;
}							t_ping;

extern t_ping				*g_ping;

void						ft_perror(char *s, int code);
int							get_host_ip(void);
void						signal_handler_alrm(int sig);
void						signal_handler(int sig);
void						loop(void);
void						fill_send_packet(t_loop *state);
void						fill_recv_msg(t_msg *msg_recv);
void						read_ping(t_msg *reply);
bool						inspect_and_print_ping(t_msg *msg_recv,
								struct timeval *tv);
void						print_ping_gen(struct timeval *tv_seq_start,
								ssize_t read_icmp_bytes, uint16_t rcv_icmp_seq);
void						print_statistics(void);
void						print_statistics_post(float rate,
								double total_time_elapsed);

#endif