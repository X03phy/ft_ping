#include "ping.h"

#include <string.h> // memset()
#include <stdio.h>

void ping_ctx_init(s_ping_ctx *ctx)
{
	memset(ctx, 0, sizeof(s_ping_ctx));

	/* Valeurs par défaut des options */
	ctx->ttl         = PING_DEFAULT_TTL;
	ctx->count       = PING_DEFAULT_COUNT;
	ctx->interval    = PING_DEFAULT_INTERVAL;
	ctx->timeout     = PING_DEFAULT_TIMEOUT;
	ctx->packet_size = PING_DEFAULT_PAYLOAD;
}
