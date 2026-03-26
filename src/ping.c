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

int ping(s_ping_ctx *ping_ctx)
{
	ping_ctx->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping_ctx->sockfd < 0)
		perror("socket");

	


	while (g_pingloop) {
		
	}

	return (0);
}
