#include "ft_ping.h"

uint16_t	checksum(void *addr, int size)
{
	uint16_t	*buff;
	uint32_t	sum;

	buff = (uint16_t *)addr;
	for (sum = 0; size > 1; size -= 2)
		sum += *buff++;
	if (size == 1)
		sum += *(uint8_t*)buff;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return (~sum);
}

void	fill_send_packet(t_loop *state)
{
	int i;

	i = 0;
	state->msg_count++;
	ft_bzero((void *)&state->pckt, sizeof(t_pkt4));
	state->pckt.hdr.type = ICMP_ECHO;
	state->pckt.hdr.un.echo.id = getpid();
	while (i < (int)PING_PACKET_DATA_SIZE - 1)
	{
		state->pckt.msg[i] = i + '0';
		i++;
	}
	state->pckt.msg[i] = 0;
	state->pckt.hdr.un.echo.sequence = state->msg_count;
	state->pckt.hdr.checksum = checksum(&state->pckt, sizeof(state->pckt));
}