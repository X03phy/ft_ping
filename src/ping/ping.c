#include "ping.h"

#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int g_pingloop = 1;

void ping_signal_handler(void)
{
	g_pingloop = 0;
}

int ping(s_ping_ctx *ctx)
{
	int sockfd;

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		perror("socket()");
		return (1);
	}
	printf("yes it works\n");

	while (g_pingloop) {
		break;
		(void)ctx;
	}

	return (0);
}
