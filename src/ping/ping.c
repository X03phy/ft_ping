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
static int ping_once(s_ping_ctx *ctx, unsigned short seq, struct timeval *tv_recv);
static double time_diff_ms_safe(struct timeval *start, struct timeval *end);

int ping_run(s_ping_ctx *ctx)
{
	int ret;

	if (ping_setup(ctx) != 0)
		return (1);

	signal(SIGINT, sig_handler); //! can fail -> to secure AND SIGNAL NOT RECOMMENDED

	print_header(ctx);
	ret = ping_loop(ctx);
	print_stats(ctx);

	close(ctx->sockfd); /* ping_cleanup() */
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
	struct timeval tv_recv;

	seq = 0;
	while (g_pingloop) {
		if (ping_once(ctx, seq, &tv_recv) != 0)
			return (1);
		seq++;
		if (ctx->count != -1 && seq >= (unsigned short)ctx->count)
			break ;
		sleep((unsigned int)ctx->interval);
	}

	return (0);
}

static int ping_once(s_ping_ctx *ctx, unsigned short seq, struct timeval *tv_recv)
{
	char buf[IP_HDR_SIZE + sizeof(s_icmp_pkt)];
	double rtt;
	s_icmp_pkt pkt;
	size_t r;
	struct sockaddr_in from;
	struct timeval tv_send;

	icmp_build(&pkt, seq);

	if (icmp_send(&pkt, ctx->sockfd, &ctx->addr) != 0)
		return (1);
	ctx->sent++;

	if (icmp_recv(&r, ctx->sockfd, buf, sizeof(buf), &from, tv_recv) != 0)
		return (0);
	ctx->received++;

	memcpy(&tv_send, pkt.data, sizeof(tv_send));
	rtt = time_diff_ms_safe(&tv_send, tv_recv);
	print_response(buf, r, &from, rtt); /* get_rtt before */

	if (ctx->rtt_min < 0 || rtt < ctx->rtt_min)
		ctx->rtt_min = rtt;
	if (rtt > ctx->rtt_max)
		ctx->rtt_max = rtt;
	ctx->rtt_sum += rtt;
	ctx->rtt_sum_sq += rtt * rtt;

	return (0);
}

static double time_diff_ms_safe(struct timeval *start, struct timeval *end)
{
	struct timeval tmp;

	tmp.tv_sec = end->tv_sec - start->tv_sec;
	tmp.tv_usec = end->tv_usec - start->tv_usec;
	if (tmp.tv_usec < 0) {
		tmp.tv_sec -= 1;
		tmp.tv_usec += 1000000;
	}

	return (tmp.tv_sec * 1000.0 + tmp.tv_usec / 1000.0);
}
