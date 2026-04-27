#include "ping.h"

#include <netdb.h> // struct addrinfo, getaddrinfo(), freeaddrinfo()
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // AF_INET, IPPROTO_ICMP, SOCK_RAW, socket()
#include <sys/time.h> // struct timeval, gettimeofday()
#include <netinet/ip.h> // IP_TTL

#include <stddef.h> // NULL
#include <stdio.h> // perror()
#include <string.h> // memset()

static int resolve_host(struct sockaddr_in *addr, const char *host);

int ping_setup(s_ping_ctx *ctx)
{
	struct timeval timeout;

	if (resolve_host(&ctx->addr, ctx->host) != 0) {
		return (1);
	}

	ctx->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ctx->sockfd == -1) {
		perror("socket()");
		return (1);
	}

	if (setsockopt(ctx->sockfd, IPPROTO_IP, IP_TTL, &ctx->ttl, sizeof(ctx->ttl)) == -1) {
		perror("setsockopt() TTL");
		return (1);
	}

	timeout.tv_sec  = (long)ctx->interval;
	timeout.tv_usec = (long)((ctx->interval - timeout.tv_sec) * 1000000);
	if (setsockopt(ctx->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
		perror("setsockopt() RCVTIMEO");
		return (1);
	}

	return (0);
}

static int resolve_host(struct sockaddr_in *addr, const char *host)
{
	struct addrinfo hints, *res;
	int ret;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;

	ret = getaddrinfo(host, NULL, &hints, &res);
	if (ret != 0) {
		dprintf(2, "getaddrinfo(): %s\n", gai_strerror(ret)); /* To change */
		return (1);
	}

	*addr = *(struct sockaddr_in *)res->ai_addr;

	freeaddrinfo(res);
	return (0);
}
