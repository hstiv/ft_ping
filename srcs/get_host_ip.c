#include "ft_ping.h"

static char	*reverse_dns_lookup(void)
{
	struct sockaddr_in	temp_addr;
	char				buff_hostname[NI_MAXHOST];
	int					status;

	status = inet_pton(AF_INET, g_ping->ip_addr, (void *)&temp_addr.sin_addr);
	if (status <= 0)
		ft_perror("inet_pton: Invalid string", 0);
	temp_addr.sin_family = AF_INET;
	temp_addr.sin_port = 0;
	status = getnameinfo((struct sockaddr *) &temp_addr,
			sizeof(struct sockaddr_in), buff_hostname,
			NI_MAXHOST, NULL, 0, NI_NAMEREQD);
	if (status != 0)
		ft_perror("getnameinfo", 0);
	return (ft_strdup(buff_hostname));
}

static int	set_ip_addr(t_ping *g_ping)
{
	struct addrinfo		*addr_ptr;
	const char			*addr_str = NULL;

	addr_ptr = g_ping->addr_res;
	while (addr_ptr)
	{
		if (addr_ptr->ai_family == AF_INET)
		{
			ft_memcpy(&(g_ping->ip4), (void *)addr_ptr->ai_addr,
				sizeof(struct sockaddr_in));
			addr_str = inet_ntop(AF_INET, &(g_ping->ip4.sin_addr),
					g_ping->ip_addr, INET_ADDRSTRLEN);
			break ;
		}
		addr_ptr = addr_ptr->ai_next;
	}
	if (!addr_str)
		ft_perror("inet_ntop", 0);
	freeaddrinfo(g_ping->addr_res);
	return (EXIT_SUCCESS);
}

int	get_host_ip(void)
{
	int					s;
	struct addrinfo		hints;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	s = getaddrinfo(g_ping->hostname, NULL, &hints, &g_ping->addr_res);
	if (s != 0)
	{
		dprintf(STDERR_FILENO, "getaddrinfo: %s\n", gai_strerror(s));
		exit (EXIT_FAILURE);
	}
	if (!g_ping->addr_res || set_ip_addr(g_ping) == EXIT_FAILURE)
	{
		dprintf (STDERR_FILENO, "%s: No address associated with hostname\n",
			g_ping->hostname);
		exit (EXIT_FAILURE);
	}
	g_ping->reversed_dns = reverse_dns_lookup();
	return (EXIT_SUCCESS);
}
