#include "ping.h"

#include <arpa/inet.h> // INET_ADDRSTRLEN, inet_ntoa(), inet_ntop()
#include <sys/socket.h> // AF_INET

#include <unistd.h> // getpid()

#include <stdio.h> // printf()
#include <math.h> // sqrt()

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
	pid_t pid;

	inet_ntop(AF_INET, &ctx->addr.sin_addr, ip, sizeof(ip));
	printf("ping %s (%s): %d data bytes",
	       ctx->hostname, ip, DATA_SIZE);
	if (ctx->flags & FLAG_VERBOSE) {
		pid = getpid();
		printf(", id 0x%04x = %d", (uint16_t)(pid & 0xFFFF), (uint16_t)(pid & 0xFFFF));
	}
	printf("\n");
}

void print_response(const t_ping_ctx *ctx, const t_icmp_reply *reply, double rtt)
{
	if (ctx->flags & FLAG_FLOOD) {
		printf("\b \b");
		return ;
	}
	printf("%zu bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
	       sizeof(t_icmp_pkt), inet_ntoa(reply->from.sin_addr),
	       reply->pkt.hdr.seq, reply->ttl, rtt);
}

void print_stats(const t_ping_ctx *ctx)
{
	double loss;
	double avg;
	double stddev;

	loss = 0.0;
	if (ctx->send != 0)
		loss = ((ctx->send - ctx->received) * 100.0) / ctx->send;
	printf("\n--- %s ping statistics ---\n", ctx->hostname);
	printf("%u packets transmitted, ", ctx->send);
	printf("%u packets received, ", ctx->received);
	printf("%.1f%% packet loss\n", loss);
	if (ctx->received == 0)
		return ;
	avg = ctx->rtt_sum / ctx-> received;
	stddev = sqrt(ctx->rtt_sum_sq / ctx->received - avg * avg);
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
	       ctx->rtt_min, avg, ctx->rtt_max, stddev);
}
