#include "ft_ping.h"

void					loop_preprocessor(t_loop *state)
{
	state->msg_count = 0;
	state->tv.tv_sec = RECV_TIMEOUT;
   	state->tv.tv_usec = 0;
	printf("PING %s (%s) %zu(%d) bytes of data.\n", g_ping->hostname, g_ping->ip_addr, PING_PACKET_DATA_SIZE + sizeof(struct ip*), PING_PACKET_SIZE);
	g_ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_ping->sockfd == -1)
	{
		dprintf(STDERR_FILENO, "ERROR: %s (sockfd = %d)\n", "socket", g_ping->sockfd);
		exit (EXIT_FAILURE);
	}
	if (setsockopt(g_ping->sockfd, SOL_IP, IP_TTL, (const void *)&g_ping->ttl, sizeof(g_ping->ttl)) == -1)
	{
		dprintf(STDERR_FILENO, "ERROR: Setting socket options to TTL failed!\n");
		exit (EXIT_FAILURE);
	}
	setsockopt(g_ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&state->tv, sizeof(state->tv));
	if (gettimeofday(&g_ping->tv_start, NULL) != 0)
	{
		dprintf(STDERR_FILENO, "%s\n", "ERROR:[gettimeofday]: One of [tv] or [tz] pointed outside the accessible address space.\n");
		exit (EXIT_FAILURE);
	}
}

void					loop(void)
{
	t_loop				state;
	t_msg				msg_recv;
	uint16_t			rcv_icmp_seq;

	loop_preprocessor(&state);
	state.send_allowed = 1;
	while (1)
	{
		fill_send_packet(&state);
		if (state.send_allowed)
		{
			if (gettimeofday(&state.tv, NULL) != 0)
			{
				dprintf(STDERR_FILENO, "%s\n", "ERROR:[gettimeofday]: One of [tv] or [tz] pointed outside the accessible address space.");
				exit (EXIT_FAILURE);
			}
		}
		g_ping->wait = true;
		alarm(INTERVAL);
		if (sendto(g_ping->sockfd, &state.pckt, sizeof(state.pckt), 0, (struct sockaddr*)&g_ping->ip4, sizeof(g_ping->ip4)) == -1)
		{
			dprintf(STDERR_FILENO, "sento error -1\n");
			exit (EXIT_FAILURE);
		}
		state.addr_len = sizeof(state.r_addr);
		fill_recv_msg(&msg_recv);
		msg_recv.read_bytes = recvmsg(g_ping->sockfd, &msg_recv.hdr, 0);
		if (msg_recv.read_bytes == 0)
		{
			dprintf(STDERR_FILENO, "%s: socket closed\n", PROG_NAME);
			exit(EXIT_FAILURE);
		}
		if (msg_recv.read_bytes == -1)
		{
			if (errno == EINTR || errno == EAGAIN)
			{
				state.send_allowed = true;
				if (g_ping->options[OPT_V])
					dprintf(STDERR_FILENO, "%s: Request timed out for icmp_seq=%hu\n", PROG_NAME, g_ping->stat.icmp_send);
			}
			else
			{
				dprintf(STDERR_FILENO, "ft_pint: recvmsg error\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			read_ping(&msg_recv);
			rcv_icmp_seq = rev_uint16(msg_recv.icmp4->icmp_seq);
			if (rcv_icmp_seq == g_ping->stat.icmp_send || msg_recv.icmp4->icmp_type != ICMP_ECHOREPLY)
			{
				
				state.send_allowed = inspect_and_print_ping(&msg_recv, &state.tv);
				if (!state.send_allowed)
					continue;
			}
			else
			{
				if (g_ping->options[OPT_V])
					dprintf(STDERR_FILENO, "%s: received icmp_seq=%hu later\n", PROG_NAME, rcv_icmp_seq);
				state.send_allowed = false;
				continue ;
			}
		}
		if (gettimeofday((struct timeval *)&g_ping->stat.tv_ping_end, NULL) == -1)
		{
			dprintf(STDERR_FILENO, "ft_pint: gettimeofday\n");
			exit(EXIT_FAILURE);
		}
		while (INTERVAL > 0 && g_ping->wait) ;
	}
}
