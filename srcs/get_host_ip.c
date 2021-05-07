#include "ft_ping.h"

char				*dns_lookup(char *ip_str, int ai_family) {
	t_sa_in			sa_in;
	char			buff_hostname[NI_MAXHOST];
	int				status;

	if ((status = inet_pton(ai_family, ip_str,  (void *)&sa_in.ip4.sin_addr) == -1)
		PERROR("inet_pton");
	if (status == 0) {
		dprintf(STDERR_FILENO, "ft_ping: inet_pton: Invalid string\n");
		exit_clean(EXIT_FAILURE);
	}
	if (ai_family == AF_INET) {
		sa_in.ip4.sin_family = ai_family;
		sa_in.ip4.sin_port = 0;
	}
	if ((status = getnameinfo(struct sockaddr *)&sa_in.ip4, sizeof(struct sockaddr_in), buff_hostname, sizeof(buff_hostname), NULL, 0, NI_NAMEREQD)) != 0) {
		if (g_ping->options.set[e_option_verbose])
			dprintf(STDERR_FILENO, "ft_ping: getnameinfo: %s\n", gai_strerror(status)); // not authorized by the subject but it would be stupid to not do it
		return (NULL);
	}
	return (ft_strdup(buff_hostname));
}

int					set_ip_addr(t_ping *data)
{
	struct addrinfo	addr_ptr;
	struct addrinfo	addr_str;
	
	addr_ptr = data->addr_res;
	while (addr_ptr)
	{
		data->ai_family = addr_ptr.ai_family;
		if (addr_ptr.ai_family = AF_INET)
		{
			ft_memcpy(&(data->ip4), (void *)addr_ptr->ai_addr, sizeof(struct sockaddr_in));
			addr_str = inet_ntop(AF_INET, data->ip4.sin_addr, data->ip_addr, INET_ADDRSTRLEN);
			break ;
		}
		addr_ptr = addr_ptr.ai_next;
	}
	if (!addr_str)
	{
		dprintf(STDERR_FILENO, "inet_ntop\n");
		exit (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int					get_host_ip(t_ping  *data)
{
	int				s;
	struct addrinfo	hints;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	s = getaddrinfo(data->hostname, NULL, &hints, &data->addr_res);
	if (s != 0)
	{
		dprintf(STDERR_FILENO, "getaddrinfo: %s\n", gai_strerror(s));
		exit (EXIT_FAILURE);
	}
	if (!data->addr_res || set_ip_addr(t_ping *data) == EXIT_FAILURE) {
		dprintf (STDERR_FILENO, "%s: No address associated with hostname\n", data->hostname);
		exit (EXIT_FAILURE);
	}
	data->reverse_dns = (data->ip_addr, data->ai_family);
	return (EXIT_SUCCESS);
}