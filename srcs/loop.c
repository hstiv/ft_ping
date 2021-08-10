#include "ft_ping.h"

void					loop_preprocessor(t_loop *state)
{
	int 				ip_ttl;

	ip_ttl = 64;
	state->msg_count = 0;
	state->tv.tv_sec = RECV_TIMEOUT;
   	state->tv.tv_usec = 0;
	printf("PING %s (%s) %zu(%d) bytes of data.\n", g_ping->hostname, g_ping->ip_addr, PING_PACKET_DATA_SIZE + sizeof(struct ip*), PING_PACKET_SIZE);
	g_ping->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (g_ping->sockfd == -1)
	{
		dprintf(STDERR_FILENO, "ERROR: %s (sockfd = %d)\n", "socket", g_ping->sockfd);
		exit (EXIT_FAILURE);
	}
	if (setsockopt(g_ping->sockfd, SOL_IP, IP_TTL, (const void *)&ip_ttl, sizeof(ip_ttl)) == -1)
	{
		dprintf(STDERR_FILENO, "ERROR: Setting socket options to TTL failed!");
		exit (EXIT_FAILURE);
	}
	setsockopt(g_ping->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&state->tv, sizeof(state->tv));
	if (gettimeofday(&g_ping->tv_start, NULL) != 0)
	{
		dprintf(STDERR_FILENO, "%s\n", "ERROR:[gettimeofday]: One of [tv] or [tz] pointed outside the accessible address space.");
		exit (EXIT_FAILURE);
	}
}

void					loop(void)
{
	t_loop				state;
	int					msg_send;

	loop_preprocessor(&state);
	state.send_allowed = 1;
	msg_send = 1;
	while (1)
	{
		fill_send_packet(&state);
		printf("%s\n", state.pckt.msg);
		if (state.send_allowed)
		{
			if (gettimeofday(&state.tv, NULL) != 0)
			{
				dprintf(STDERR_FILENO, "%s\n", "ERROR:[gettimeofday]: One of [tv] or [tz] pointed outside the accessible address space.");
				// exit (EXIT_FAILURE);
			}
		}
		printf("%ld\n", sizeof(state.pckt));
		sendto(g_ping->sockfd, &state.pckt, sizeof(state.pckt), 0, NULL, 0);
		// if (sendto(g_ping->sockfd, &state.pckt, sizeof(state.pckt), 0, (struct sockaddr*)&g_ping->ip4, sizeof(g_ping->ip4)) == -1)
		// {
		// 	dprintf(STDERR_FILENO, "sento error -1\n");
		// 	// exit (EXIT_FAILURE);
		// }
		// state.addr_len = sizeof(state.r_addr);
		// if (recvfrom(g_ping->sockfd, &state.pckt, sizeof(state.pckt), 0, (struct sockaddr*)&state.r_addr, &state.addr_len) <= 0 && state.msg_count > 1) 
        // {
		// 	dprintf(STDERR_FILENO, "\nPacket receive failed!\n");
		// 	exit (EXIT_FAILURE);
        // }
		// printf("ggtwgtr\n");
		state.send_allowed = 1;
	}
}
