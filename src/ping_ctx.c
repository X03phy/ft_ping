#include "ping.h"

#include <string.h> // memset()
#include <stdio.h>

void ping_ctx_init(s_ping_ctx *ping_ctx)
{
	memset(ping_ctx, 0, sizeof(s_ping_ctx));

	/* Valeurs par défaut des options */
	ping_ctx->ttl         = PING_DEFAULT_TTL;
	ping_ctx->count       = PING_DEFAULT_COUNT;
	ping_ctx->interval    = PING_DEFAULT_INTERVAL;
	ping_ctx->timeout     = PING_DEFAULT_TIMEOUT;
	ping_ctx->packet_size = PING_DEFAULT_PAYLOAD;

	/* État initial */
	ping_ctx->sockfd = -1;
	//ping_ctx->seq    = 0;
//	ping_ctx->pid    = (uint16_t)getpid();
//	ping_ctx->flags  |= FLAG_RUNNING;
}

int ping_ctx_setup(s_ping_ctx *ctx)
{
	if (dns_lookup(ctx->ip, ctx->target) != 0) {
		fprintf(stderr, PING_UNKNOWN_HOST_FORMAT,
			ctx->progname, ctx->target);
		return (1);
	}

	return (0);
}
