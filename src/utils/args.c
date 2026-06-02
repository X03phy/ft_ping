#include "ping.h"

#include <string.h> // strcmp()

static void parse_hostname(t_ping_ctx *ctx, char *arg)
{
	if (!ctx->hostname)
		ctx->hostname = arg;
}

static void parse_flag(t_ping_ctx *ctx, char **argv, int i)
{
	if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0)
		ctx->flags |= FLAG_HELP;
	else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
		ctx->flags |= FLAG_VERBOSE;
}

int parse_args(t_ping_ctx *ctx, int argc, char **argv)
{
	int i;

	if (argc < 2) {
		print_help(argv[0]);
		return (1);
	}
	ctx->progname = argv[0];
	i = 1;
	while (argv[i]) {
		if (argv[i][0] == '-')
			parse_flag(ctx, argv, i);
		else
			parse_hostname(ctx, argv[i]);
		i++;
	}

	return (0);
}
