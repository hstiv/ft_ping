#include "ft_ping.h"

// static void			init_params(t_ping *data, int argc, char **argv)
// {
// 	int				s;

// 	s = getaddrinfo(argv[1], NULL, NULL, &data->res);
// 	if (s != 0)
// 	{
// 		ft_putstr("getaddrinfo: ");
// 		ft_putendl(gai_strerror(s));
// 		exit(EXIT_FAILURE);
// 	}
// 	if (gettimeofday(&data->tv, &data->tz) != 0)
// 	{	
// 		ft_putendl("[gettimeofday]: One of [tv] or [tz] pointed outside the accessible address space.");
// 		exit(EXIT_FAILURE);
// 	}
// 	data->pid = getpid();
// 	data->uid = getuid();
// 	// if (inet_pton(AF_INET, argv[1], data->ipv4_addr) == NULL)
// 	// {
// 	// 	ft_putendl("[inet_ntop]: [af] was not a valid address family.");
// 	// 	exit(EXIT_FAILURE);
// 	// }
	// sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
// }

// int					main(int argc, char	**argv)
// {
// 	int				i;
// 	t_ping			data;

// 	i = 1;
// 	if (argc < 2)
// 	{
// 		ft_putstr(USAGE);
// 		exit (EXIT_FAILURE);
// 	}
// 	init_params(&data, argc, argv);
// 	while (i < argc)
// 		printf("*-> %s\n", argv[i++]);
// 	exit (EXIT_SUCCESS);
// }

int					main(int argc, char	**argv)
{
	int				sockfd;
	int				s;
	char			buffer[80];
	struct	iovec	iov[1];
	struct	msghdr	msg;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    	error("setsockopt(SO_REUSEADDR) failed");
	memset(&msg, 0, sizeof(msg));
	memset(iov, 0, sizeof(iov));

	/*************************************************/
	/* The recvmsg() call will NOT block unless a    */
	/* non-zero length data buffer is specified      */
	/*************************************************/
	iov[0].iov_base = buffer;
	iov[0].iov_len  = sizeof(buffer);
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;

	/*************************************************/
	/* Wait for the descriptor to arrive             */
	/*************************************************/
	printf("Waiting on recvmsg\n");
	s = recvmsg(sockfd, &msg, 0);
	exit(EXIT_SUCCESS);
}