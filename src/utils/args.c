#include "ping.h"

int parse_args(s_ping_ctx *ctx, int argc, char **argv)
{
	if (argc < 2) {
		print_help(argv[0]);
		return (1);
	}

	ctx->progname = argv[0];
	ctx->host = argv[1];

	return (0);
}
