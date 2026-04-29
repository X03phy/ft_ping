#include "ping.h"

#include <arpa/inet.h> // inet_ntop()
#include <stdio.h> // print()
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

void print_header(const s_ping_ctx *ctx)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &ctx->addr.sin_addr, ip, sizeof(ip));
	printf("PING %s (%s): %d data bytes\n", ctx->host, ip, DATA_SIZE);
}

void print_response(char *buf, size_t r, struct in_addr *in, double rtt)
{
	s_icmp_pkt *pkt;
	size_t size;
	struct ip *ip;

	ip = (struct ip *)buf;
	size = (ip->ip_hl * 4);
	pkt = (s_icmp_pkt *)(buf + size);
	size = r - size;

	printf("%zu bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
		size,
		inet_ntoa(from->sin_addr),
		ntohs(pkt->hdr.seq),
		ip->ip_ttl,
		rtt
	);
}

void print_stats(const s_ping_ctx *ctx)
{
	double avg, loss, stddev;

	printf("\n--- %s ping statistics ---\n", ctx->host);

	loss = (ctx->sent != 0) ? ((ctx->sent - ctx->received) * 100.0) / ctx->sent : 0;
	printf("%u packets transmitted, %u packets received, %.1f%% packet loss\n", ctx->sent, ctx->received, loss);

	if (ctx->received != 0) {
		avg = (ctx->received != 0) ? ctx->rtt_sum / ctx-> received : 0;
		stddev = (ctx->received != 0) ? sqrt(ctx->rtt_sum_sq / ctx->received - avg * avg) : 0;
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", ctx->rtt_min, avg, ctx->rtt_max, stddev);
	}
}
