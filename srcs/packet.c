#include "ft_ping.h"

void	fill_packet4(struct s_pkt4 *pkt, time_t timestamp)
{
	ft_bzero(ping_pkt, sizeof(struct s_ping_pkt4));
	ping_pkt->icmp.icmp_type = ICMP_ECHO;
	ping_pkt->icmp.icmp_code = PING_ICMP_ECHO_CODE;
	ping_pkt->icmp.icmp_id = BSWAP16((uint16_t)getpid());
	g_ping->stat.icmp_send++;
	ping_pkt->icmp.icmp_seq = BSWAP16(g_ping->stat.icmp_send);
	ping_pkt->timestamp = BSWAP64(timestamp);
	ft_memcpy(&ping_pkt->icmp.icmp_dun, &timestamp, sizeof(timestamp));
	ping_pkt->icmp.icmp_cksum = checksum(ping_pkt, sizeof(*ping_pkt));
}