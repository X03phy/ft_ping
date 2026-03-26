#include "ping.h"

int parse_args(s_ping_ctx *ctx, int argc, char **argv)
{
	ctx->progname = argv[0];
	ctx->target = argv[1];
	(void)argc;
	(void)argv;

	return (0);
}
