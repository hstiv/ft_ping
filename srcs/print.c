#include "ft_ping.h"

void	update_stats(double tv_seq_diff)
{
	if (g_ping->stat.icmp_rcv == 1)
		g_ping->stat.rtt_min = tv_seq_diff;
	else
	{
		if (g_ping->stat.rtt_min > tv_seq_diff)
			g_ping->stat.rtt_min = tv_seq_diff;
	}
	if (g_ping->stat.rtt_max < tv_seq_diff)
		g_ping->stat.rtt_max = tv_seq_diff;
	g_ping->stat.rtt_sum += tv_seq_diff;
	g_ping->stat.rtt_square_sum += tv_seq_diff * tv_seq_diff;
}

bool	inspect_and_print_ping(t_msg *msg_recv, struct timeval *tv)
{
	uint16_t	rcv_icmp_seq;
	struct icmp *icmp_send_request;

	if (msg_recv->icmp4->icmp_type == ICMP_ECHO)
		return (false);
	rcv_icmp_seq = rev_uint16(msg_recv->icmp4->icmp_seq);
	if (msg_recv->icmp4->icmp_type != ICMP_ECHOREPLY) {
		if ((size_t)msg_recv->read_bytes >= ((void *)msg_recv->icmp4 - (void *)&msg_recv->buff) + sizeof(struct ip) + sizeof(struct icmp))
		{
			icmp_send_request = (struct icmp *)(msg_recv->icmp4->icmp_data + sizeof(struct ip));
			rcv_icmp_seq = rev_uint16(icmp_send_request->icmp_seq);
		}
		else if (g_ping->options[OPT_V])
			dprintf(STDERR_FILENO, "Missing previous ICMP header in msg_recv\n");
		if (rcv_icmp_seq == g_ping->stat.icmp_send)
			return (true);
		return (false);
	}
	print_ping_gen(tv, msg_recv->read_bytes - sizeof(struct ip), rcv_icmp_seq);
	return (true);
}

void	print_ping_gen(struct timeval *tv_seq_start, ssize_t read_icmp_bytes, uint16_t rcv_icmp_seq)
{
	struct timeval tv_seq_end;
	double tv_seq_diff;

	g_ping->stat.icmp_rcv++;
	if (gettimeofday(&tv_seq_end, NULL) == -1)
	{
		dprintf(STDERR_FILENO, "ft_ping: gettimeofday\n");
		exit(EXIT_FAILURE);
	}
	tv_seq_diff = (double)(tv_seq_end.tv_sec - tv_seq_start->tv_sec) * 1000.0 + (double)(tv_seq_end.tv_usec - tv_seq_start->tv_usec) / 1000.0;
	printf("%zd bytes from %s (%s): icmp_seq=%hu ttl=%d time=%.2lf ms\n", read_icmp_bytes, g_ping->reversed_dns, g_ping->ip_addr, rcv_icmp_seq, g_ping->ttl, tv_seq_diff);
	update_stats(tv_seq_diff);
}

void	print_statistics()
{
	printf("\n--- %s ping statistics ---\n", g_ping->hostname);
}
