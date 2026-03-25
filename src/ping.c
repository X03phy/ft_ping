#include "ping.h"

#include "icmp.h"
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int g_pingloop = 1;

void ping_signal_handler()
{
	g_pingloop = 0;
}

int ping(int argc, char **argv)
{
	//int sockfd;

	//sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	//if (sockfd < 0) {
	//	perror("socket()");
	//	return (0);
	//}

	//signal(SIGINT, ping_signal_handler);
	//send_ping();
	//close(sockfd);
	(void)argc;
	(void)argv;
	return (0);
}
