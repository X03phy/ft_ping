#include "ping.h"

#include <arpa/inet.h> // inet_ntop()
#include <stdio.h> // print()
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

static double time_diff_ms_safe(struct timeval *start, struct timeval *end);

void print_header(const s_ping_ctx *ctx)
{
	char ip[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &ctx->addr.sin_addr, ip, sizeof(ip));
	printf("PING %s (%s): %d data bytes\n", ctx->host, ip, DATA_SIZE);
}

double print_response(char *buf, size_t r, struct sockaddr_in *from)
{
	double rtt;
	size_t size;
	s_icmp_pkt *pkt;
	struct ip *ip;
	struct timeval tv_send, tv_recv;

	gettimeofday(&tv_recv, NULL);

	ip = (struct ip *)buf;
	pkt = (s_icmp_pkt *)(buf + (ip->ip_hl * 4));
	size = r - (ip->ip_hl * 4);

	memcpy(&tv_send, pkt->data, sizeof(tv_send));
	rtt = time_diff_ms_safe(&tv_send, &tv_recv);

	printf("%zu bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
		size,
		inet_ntoa(from->sin_addr),
		ntohs(pkt->hdr.seq),
		ip->ip_ttl,
		rtt
	);

	return (rtt);
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
