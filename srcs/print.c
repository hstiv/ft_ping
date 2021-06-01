#include "ft_ping.h"

void				print_ping_header(void)
{
	if (g_ping->ai_family == AF_INET)
		printf("PING %s (%s) %zu(%d) bytes of data.\n", g_ping->hostname, g_ping->ip_addr, PING_PACKET_DATA_SIZE + sizeof(struct ip*), PING_PACKET_SIZE);
	else
		printf("PING %s(%s (%s)) %zu data bytes\n", g_ping->hostname, g_ping->reversed_dns, g_ping->hostname, sizeof(struct icmp6_hdr) + PING_PACKET_DATA_SIZE);
}