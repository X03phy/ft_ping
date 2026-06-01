#include "ping.h"

#include <signal.h> // SIGINT, signal()

#include <unistd.h> // close()
#include <stddef.h> // NULL
#include <string.h> // memset()
#include <netdb.h> // struct addrinfo
#include <stdio.h> // dprintf()

int g_pingloop = 1;

static void sig_handler(int dummy)
{
	(void)dummy;
	g_pingloop = 0;
}

static int ping_once(t_ping_ctx *ctx, unsigned short seq)
{
	t_icmp_pkt pkt;
	double rtt = 0;

	pkt = icmp_build(seq);
	if (icmp_send(&pkt, ctx) != 0)
		return (1);
	ctx->sent++;
	if (icmp_recv(&pkt, ctx) != 0)
		return (0);
	ctx->received++;
	print_response(ctx, &pkt, rtt);
	//memcpy(&tv_send, pkt.data, sizeof(tv_send));
	//rtt = time_diff_ms(&tv_send, tv_recv);
	//if (ctx->rtt_min < 0 || rtt < ctx->rtt_min)
	//	ctx->rtt_min = rtt;
	//if (rtt > ctx->rtt_max)
	//	ctx->rtt_max = rtt;
	//ctx->rtt_sum += rtt;
	//ctx->rtt_sum_sq += rtt * rtt;
	return (0);
}

static int ping_loop(t_ping_ctx *ctx)
{
	unsigned short seq;
	//struct timeval tv_recv;

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

int ping_run(t_ping_ctx *ctx)
{
	int ret;

	if (ping_setup(ctx) != 0)
		return (1);
	signal(SIGINT, sig_handler); //! can fail -> to secure AND SIGNAL NOT RECOMMENDED
	print_header(ctx);
	ret = ping_loop(ctx);
	print_stats(ctx);
	ping_cleanup(ctx);
	return (ret);
}
