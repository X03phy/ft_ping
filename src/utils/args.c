#include "ping.h"

int parse_args(t_ping_ctx *ctx, int argc, char **argv)
{
	if (argc < 2) {
		print_help(argv[0]);
		return (1);
	}

	ctx->progname = argv[0];
	ctx->hostname = argv[1];

	return (0);
}
