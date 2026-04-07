#include "ping.h"

// #include <arpa/inet.h> // inet_ntop()
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

void print_header(const s_ping_ctx *ctx)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &ctx->addr.sin_addr, ip, sizeof(ip));
	printf("PING %s (%s): %d data bytes\n", ctx->host, ip, DATA_SIZE);
}



