#include "ping.h"

#include <arpa/inet.h> // inet_ntop()
#include <stdio.h> // print()
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

void print_help(const char *progname)
{
	printf("Usage:\n");
	printf("  %s [options] hostname\n", progname);
	printf("\n");
	printf("Options:\n");
	printf("  -v\n");
	printf("  -h, -?\n");
	printf("  -f\n");
	printf("  -l\n");
	printf("  -n\n");
	printf("  -w\n");
	printf("  -W\n");
	printf("  -p\n");
	printf("  -r\n");
	printf("  -s\n");
	printf("  -T\n");
	printf("  --ttl\n");
	printf("  --ip-timestamp\n");
	printf("\n");
}

void print_header(const t_ping_ctx *ctx)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &ctx->addr.sin_addr, ip, sizeof(ip));
	printf("ft_ping %s (%s): %d data bytes\n", ctx->hostname, ip, DATA_SIZE);
}

void print_response(const t_icmp_reply *reply, double rtt)
{
	printf("%zu bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
		sizeof(t_icmp_pkt),
		inet_ntoa(reply->from.sin_addr),
		reply->pkt.hdr.seq,
		reply->ttl,
		rtt
	);
}

void print_stats(const t_ping_ctx *ctx)
{
	double avg, loss, stddev;

	printf("\n--- %s ping statistics ---\n", ctx->hostname);

	loss = (ctx->send != 0) ? ((ctx->send - ctx->received) * 100.0) / ctx->send : 0;
	printf("%u packets transmitted, %u packets received, %.1f%% packet loss\n", ctx->send, ctx->received, loss);

	if (ctx->received != 0) {
		avg = (ctx->received != 0) ? ctx->rtt_sum / ctx-> received : 0;
		stddev = (ctx->received != 0) ? sqrt(ctx->rtt_sum_sq / ctx->received - avg * avg) : 0;
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", ctx->rtt_min, avg, ctx->rtt_max, stddev);
	}
}
