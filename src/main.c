#include "ping.h"

#include <stddef.h> // NULL

static void ping_init(s_ping_ctx *ctx);

int main(int argc, char **argv)
{
	s_ping_ctx ctx;

	ping_init(&ctx);

	if (parse_args(&ctx, argc, argv) != 0)
		return (1);

	if (ping_run(&ctx) != 0)
		return (2);

	return (0);
}

static void ping_init(s_ping_ctx *ctx)
{
	ctx->progname = NULL;
	ctx->host = NULL;

	ctx->count    = -1;       // -1 = infini (comme le vrai ping)
	ctx->interval = 1.0;
	// ctx->ttl      = 64;       // TTL par défaut standard
	// ctx->flags    = 0;
	ctx->sent     = 0;
	ctx->received = 0;
	ctx->rtt_min  = -1.0;
	ctx->rtt_max  = -1.0;
	ctx->rtt_sum  = 0.0;
	ctx->rtt_sum_sq = 0.0;
}
