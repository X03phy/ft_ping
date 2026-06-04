#include "ping.h"

#include <sys/time.h> // struct timeval, gettimeofday()
#include <unistd.h> // usleep()

#include <stddef.h> // NULL
#include <stdio.h> // perror(), printf()
#include <string.h> // memset()

static void update_rtt(t_ping_ctx *ctx, int rtt)
{
	if (ctx->rtt_min < 0 || rtt < ctx->rtt_min)
		ctx->rtt_min = rtt;
	if (rtt > ctx->rtt_max)
		ctx->rtt_max = rtt;
	ctx->rtt_sum += rtt;
	ctx->rtt_sum_sq += rtt * rtt;
}

static int ping_once(t_ping_ctx *ctx, unsigned short seq)
{
	t_icmp_pkt pkt;
	t_icmp_reply reply;
	struct timeval tv_send;
	double rtt;

	pkt = icmp_build(seq);
	if (icmp_send(ctx, &pkt) != 0)
		return (1);
	ctx->send++;
	if (ctx->flags & FLAG_FLOOD)
		printf(".");
	if (icmp_recv(ctx, &reply) != 0)
		return (0);
	ctx->received++;
	memcpy(&tv_send, reply.pkt.data, sizeof(tv_send));
	rtt = time_diff_ms(&tv_send, &reply.tv_recv);
	print_response(ctx, &reply, rtt);
	update_rtt(ctx, rtt);
	return (0);
}

static int ping_loop(t_ping_ctx *ctx)
{
	unsigned short seq;
	struct timeval tv_before;
	struct timeval tv_after;
	long elapsed_us;
	long wait_us;

	seq = 0;
	while (g_pingloop) {
		gettimeofday(&tv_before, NULL);
		if (ping_once(ctx, seq) != 0)
			return (1);
		seq++;
		if (ctx->count != -1 && seq >= (unsigned short)ctx->count)
			break ;
		gettimeofday(&tv_after, NULL);
		elapsed_us = (tv_after.tv_sec  - tv_before.tv_sec) * 1000000L
		             + (tv_after.tv_usec - tv_before.tv_usec);
		wait_us = (long)(ctx->interval * 1000000L) - elapsed_us;
		if (wait_us > 0)
			usleep((useconds_t)wait_us);
	}
	return (0);
}

int ping_run(t_ping_ctx *ctx)
{
	int ret;

	if (ping_setup(ctx) != 0)
		return (1);
	if (signal_setup() != 0) {
		ping_cleanup(ctx);
		return (1);
	}
	print_header(ctx);
	ret = ping_loop(ctx);
	print_stats(ctx);
	ping_cleanup(ctx);
	return (ret);
}
