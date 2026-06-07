#include "ping.h"

#include <arpa/inet.h> // inet_ntoa()

#include <stdio.h> // printf()
void print_verbose(const t_ping_ctx *ctx, const t_icmp_reply *reply,
                    double rtt)
{
	(void)rtt;
	(void)ctx;

	printf("%zu bytes from %s: Error Message\n",
	       sizeof(t_icmp_pkt), inet_ntoa(reply->from.sin_addr));
	printf("IP header dump:\n");
	printf("%02x\n", *(unsigned char *)reply->ip);
}
