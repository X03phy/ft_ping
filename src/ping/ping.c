#include "ping.h"

#include <signal.h> // SIGINT, signal()

#include <unistd.h> // close()
#include <stddef.h> // NULL
#include <string.h> // memset()
#include <netdb.h> // struct addrinfo
#include <stdio.h> // dprintf()

int g_pingloop = 1;

static void sig_handler(int dummy);
static int ping_loop(s_ping_ctx *ctx);
static int ping_once(s_ping_ctx *ctx, unsigned short seq);

int ping_run(s_ping_ctx *ctx)
{
	int ret;

	if (ping_setup(ctx) != 0) {
		return (1);
	}

	signal(SIGINT, sig_handler);

	print_header(ctx);
	ret = ping_loop(ctx); /* Handle error here */
	print_stats(ctx);

	close(ctx->sockfd); /* ping_cleanup */
	return (ret);
}

static void sig_handler(int dummy)
{
	(void)dummy;
	g_pingloop = 0;
}

static int ping_loop(s_ping_ctx *ctx)
{
	unsigned short seq;

	seq = 0;
	while (g_pingloop) {
		if (ping_once(ctx, seq) != 0)
			return (1);
		seq++;
		if (ctx->count != -1 && seq >= (unsigned short)ctx->count)
			break ;
		sleep((unsigned int)ctx->interval);
	}

	return (0);
}

static int ping_once(s_ping_ctx *ctx, unsigned short seq)
{
	double rtt;
	s_icmp_pkt pkt;
	char buf[IP_HDR_SIZE + sizeof(s_icmp_pkt)];
	size_t r;
	struct sockaddr_in from;

	icmp_build(&pkt, seq);

	if (icmp_send(&pkt, ctx->sockfd, &ctx->addr) != 0)
		return (1);

	ctx->sent++;

	if (icmp_recv(&r, ctx->sockfd, buf, sizeof(buf), &from) != 0)
		return (1);

	ctx->received++;
	rtt = print_response(buf, r, &from);
	if (ctx->rtt_min < 0 || rtt < ctx->rtt_min)
		ctx->rtt_min = rtt;
	if (rtt > ctx->rtt_max)
		ctx->rtt_max = rtt;
	ctx->rtt_sum += rtt;
	ctx->rtt_sum_sq += rtt * rtt;

	return (0);
}
