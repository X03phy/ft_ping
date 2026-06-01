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

static int ping_once(t_ping_ctx *ctx, unsigned short seq)
{
	t_icmp_pkt pkt;
	t_icmp_reply reply;
	struct timeval tv_send;
	double rtt;

	pkt = icmp_build(seq);
	if (icmp_send(&pkt, ctx) != 0)
		return (1);
	ctx->send++;
	if (icmp_recv(&reply, ctx) != 0)
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

	seq = 0;
	while (g_pingloop) {
		if (ping_once(ctx, seq) != 0)
			return (1);
		seq++;
		if (ctx->count != -1 && seq >= (unsigned short)ctx->count)
			break ;
		sleep((unsigned int)ctx->interval); //TODO Replace with usleep()
	}
	return (0);
}

int ping_run(t_ping_ctx *ctx)
{
	int ret;
	struct sigaction sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction()");
		return (1);
	}
	if (ping_setup(ctx) != 0)
		return (1);
	print_header(ctx);
	ret = ping_loop(ctx);
	print_stats(ctx);
	ping_cleanup(ctx);
	return (ret);
}
