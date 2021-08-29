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

void	read_ping(t_msg *msg_recv) {
	struct ip	*ip_hdr;
	ssize_t		ip_icmp_offset;

	if ((size_t)msg_recv->read_bytes < sizeof(struct ip)) {
		dprintf(STDERR_FILENO, "%s: IP header from echo msg_recv truncated\n", PROG_NAME);
		exit(EXIT_FAILURE);
	}
	ip_hdr = (struct ip *)msg_recv->buff;
	ip_icmp_offset = ip_hdr->ip_hl << 2;
	if (ip_hdr->ip_p != IPPROTO_ICMP || (size_t)msg_recv->read_bytes < ip_icmp_offset + sizeof(struct icmp)) {
		dprintf(STDERR_FILENO, "%s: ICMP header from echo msg_recv truncated\n", PROG_NAME);
		exit(EXIT_FAILURE);
	}
	msg_recv->icmp4 = (struct icmp *)(msg_recv->buff + ip_icmp_offset);
}

void	fill_send_packet(t_loop *state)
{
	int i;

	i = 0;
	state->msg_count++;
	ft_bzero((void *)&state->pckt, sizeof(t_pkt4));
	state->pckt.hdr.type = ICMP_ECHO;
	state->pckt.hdr.un.echo.id = getpid();
	g_ping->stat.icmp_send++;
	while (i < (int)PING_PACKET_DATA_SIZE - 1)
	{
		state->pckt.msg[i] = i + '0';
		i++;
	}
	state->pckt.msg[i] = 0;
	state->pckt.hdr.un.echo.sequence = state->msg_count;
	state->pckt.hdr.checksum = checksum(&state->pckt, sizeof(state->pckt));
}

void	fill_recv_msg(t_msg *msg_recv)
{
	msg_recv->hdr.msg_name = NULL;
	msg_recv->hdr.msg_iov = &msg_recv->iov;
	msg_recv->iov.iov_base = msg_recv->buff;
	msg_recv->iov.iov_len = sizeof(msg_recv->buff);
	msg_recv->hdr.msg_iovlen = 1;
	msg_recv->hdr.msg_control = &msg_recv->ctrl;
	msg_recv->hdr.msg_controllen = sizeof(msg_recv->ctrl);
}
