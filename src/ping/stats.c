#include "ping.h"

#include <arpa/inet.h> // inet_ntop()
#include <stdio.h> // print()
#include <sys/time.h>
#include <unistd.h>

void print_header(const s_ping_ctx *ctx)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &ctx->addr.sin_addr, ip, sizeof(ip));
	printf("PING %s (%s): %d data bytes\n", ctx->host, ip, DATA_SIZE);
}

void print_stats(const s_ping_ctx *ctx)
{
	double loss;

	printf("\n--- %s ping statistics ---\n", ctx->host);

	if (ctx->sent == 0)
		loss = 0.0;
	else
		loss = ((ctx->sent - ctx->received) * 100.0)
			/ ctx->sent;

	printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n", ctx->sent, ctx->received, loss);

}

//void print_response(const s_ping_ctx *ctx)
//{

//}