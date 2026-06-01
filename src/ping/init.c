#include "ping.h"

#include <stddef.h> // NULL

void ping_init(t_ping_ctx *ctx)
{
	ctx->progname = NULL;
	ctx->hostname = NULL;
	ctx->count    = -1;       // -1 = infini (comme le vrai ping)
	ctx->interval = 1.0;
	ctx->ttl      = 64;       // TTL par défaut standard
	// ctx->flags    = 0;
	ctx->sent     = 0;
	ctx->received = 0;
	ctx->rtt_min  = -1.0;
	ctx->rtt_max  = -1.0;
	ctx->rtt_sum  = 0.0;
	ctx->rtt_sum_sq = 0.0;
}
