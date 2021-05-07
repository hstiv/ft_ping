#include "ft_ping.h"

const char			g_valid_opt = "vh";

static int			set_option(t_ping *ping, char c)
{
	if (ft_strchr(g_valid_opt, argv[i][j]) == NULL)
		return (EXIT_FAILURE);
	if (c == 'v')
		ping->options[OPT_V] = 1;
	if (c == 'h')
		ping->options[OPT_H] = 1;
}

static int			parse_options(int argc, char *argv, t_ping *ping)
{
	int				i;
	int				j;

	i = 0;
	while (i++ < argc)
	{
		if (argv[i][0] == '-')
		{
			j = 0;
			while (argv[i][++j] != '\0')
				if (set_option(ping, argv[i][j]) == EXIT_FAILURE)
					return (EXIT_FAILURE);
		}
		else
		{
			ping->hostname = ft_strdup(argv[i]);
			return (EXIT_SUCCESS);
		}
	}
	return (EXIT_FAILURE);
}

static void			init_params(t_ping *data, int argc, char **argv)
{
	int				s;

	s = 0;
	
	data->pid = getpid();
	data->uid = getuid();
	// if (inet_pton(AF_INET, argv[1], data->ipv4_addr) == NULL)
	// {
	// 	dprintf(EXIT_FAILURE, "%s\n", "[inet_ntop]: [af] was not a valid address family.");
	// 	exit(EXIT_FAILURE);
	// }
	if (gettimeofday(&data->tv, &data->tz) != 0)
	{	
		dprintf(STDERR_FILENO, "%s\n", "[gettimeofday]: One of [tv] or [tz] pointed outside the accessible address space.");
		exit (EXIT_FAILURE);
	}
	data->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (get_host_ip(data) == EXIT_FAILURE)
		exit (EXIT_FAILURE);
}

int					main(int argc, char	**argv)
{
	t_ping			data;
	int				i;

	i = 0;
	if (argc <= 1)
		return (EXIT_FAILURE);
	while (i < OPT_SIZE)
		data->options[i++] = 0;
	if (parse_options(argc, argv, &data) == EXIT_FAILURE)
	{
		dprintf(STDERR_FILENO, "%s\n", USAGE);
		exit (EXIT_FAILURE);
	}
	init_params(&data, argc, argv);
	exit (EXIT_SUCCESS);
}

// int					main(int argc, char	**argv)
// {
// 	int				sockfd;
// 	int				s;
// 	char			buffer[80];
// 	struct	iovec	iov[1];
// 	struct	msghdr	msg;

// 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
//     	error("setsockopt(SO_REUSEADDR) failed");
// 	memset(&msg, 0, sizeof(msg));
// 	memset(iov, 0, sizeof(iov));

// 	/*************************************************/
// 	/* The recvmsg() call will NOT block unless a    */
// 	/* non-zero length data buffer is specified      */
// 	/*************************************************/
// 	iov[0].iov_base = buffer;
// 	iov[0].iov_len  = sizeof(buffer);
// 	msg.msg_iov     = iov;
// 	msg.msg_iovlen  = 1;

// 	/*************************************************/
// 	/* Wait for the descriptor to arrive             */
// 	/*************************************************/
// 	printf("Waiting on recvmsg\n");
// 	s = recvmsg(sockfd, &msg, 0);
// 	exit(EXIT_SUCCESS);
// }

// #include "ft_ping.h"


// int				main(void)
// {
// 	struct addrinfo hints;
// 	struct addrinfo *result;
// 	int s = 0;
// 	char			*addr = "yahoo.com";

// 	/* Obtain address(es) matching host/port */

// 	ft_memset(&hints, 0, sizeof(struct addrinfo));
// 	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
// 	hints.ai_socktype = 0; /* Datagram socket */
// 	hints.ai_flags = 0;
// 	hints.ai_protocol = 0;          /* Any protocol */

// 	s = getaddrinfo(addr, NULL, &hints, &result);
// 	if (s != 0)
// 	{
// 		dprintf(1, "getaddrinfo: %s\n", gai_strerror(s));
// 		exit(EXIT_FAILURE);
// 	}
// 	// struct addrinfo {
//     //            int              ai_flags;
//     //            int              ai_family;
//     //            int              ai_socktype;
//     //            int              ai_protocol;
//     //            socklen_t        ai_addrlen;
//     //            struct sockaddr *ai_addr;
//     //            char            *ai_canonname;
//     //            struct addrinfo *ai_next;
//     //        };
// 	while (result)
// 	{
// 		printf(
// 				TEST,
// 				result->ai_flags,
// 				result->ai_family,
// 				result->ai_socktype,
// 				result->ai_protocol,
// 				result->ai_addrlen,
// 				result->ai_canonname
// 			);
// 		result = result->ai_next;
// 	}
	
// 	exit (EXIT_SUCCESS);
// }