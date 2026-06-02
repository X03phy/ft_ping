#include "ping.h"

#include <signal.h> // struct sigaction, SIGINT, sigaction(), sigemptyset()
#include <sys/time.h> // struct timeval
#include <unistd.h> // usleep()

#include <stddef.h> // NULL
#include <stdio.h> // perror()
#include <string.h> // memset()

int g_pingloop = 1;

static void sig_handler(int dummy)
{
	(void)dummy;
	g_pingloop = 0;
}

static double time_diff_ms(struct timeval *start, struct timeval *end)
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
	if (icmp_recv(ctx, &reply) != 0)
		return (0);
	ctx->received++;
	memcpy(&tv_send, reply.pkt.data, sizeof(tv_send));
	rtt = time_diff_ms(&tv_send, &reply.tv_recv);
	print_response(&reply, rtt);
	if (ctx->rtt_min < 0 || rtt < ctx->rtt_min)
		ctx->rtt_min = rtt;
	if (rtt > ctx->rtt_max)
		ctx->rtt_max = rtt;
	ctx->rtt_sum += rtt;
	ctx->rtt_sum_sq += rtt * rtt;
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
		elapsed_us = (tv_after.tv_sec  - tv_before.tv_sec)  * 1000000L
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
	struct sigaction sa;

	if (ping_setup(ctx) != 0)
		return (1);
	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction()");
		ping_cleanup(ctx);
		return (1);
	}
	print_header(ctx);
	ret = ping_loop(ctx);
	print_stats(ctx);
	ping_cleanup(ctx);
	return (ret);
}
