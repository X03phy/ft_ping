#include "ping.h"

#include <stdio.h> // dprintf()
#include <string.h> // strcmp()

#include <stdlib.h> // atoi(); //! To change

static int parse_hostname(t_ping_ctx *ctx, char *arg)
{
	if (!ctx->hostname) {
		ctx->hostname = arg;
		return (0);
	}
	return (1);
}

static int parse_flag(t_ping_ctx *ctx, int argc, char **argv, int *i)
{
	(void)argc;

	if (strcmp(argv[*i], "-h") == 0 || strcmp(argv[*i], "-?") == 0 || strcmp(argv[*i], "--help") == 0)
		ctx->flags |= FLAG_HELP;
	else if (strcmp(argv[*i], "-v") == 0 || strcmp(argv[*i], "--verbose") == 0)
		ctx->flags |= FLAG_VERBOSE;
	else if (strcmp(argv[*i], "-f") == 0 || strcmp(argv[*i], "--flood") == 0) {
		ctx->flags |= FLAG_FLOOD;
		ctx->interval = 0.01;
	}
	else if (strcmp(argv[*i], "-w") == 0 || strcmp(argv[*i], "--timeout") == 0) {
		ctx->flags |= FLAG_TIMEOUT;
		(*i)++;
		ctx->timeout = atoi(argv[*i]);
	}
	else if (strcmp(argv[*i], "-n") == 0 || strcmp(argv[*i], "--numeric") == 0)
		ctx->flags |= FLAG_NUMERIC;
	else if (strcmp(argv[*i], "-r") == 0 || strcmp(argv[*i], "--ignore-routing") == 0)
		ctx->flags |= FLAG_IGNORE_ROUTING;

	else if (strcmp(argv[*i], "-l") == 0 || strcmp(argv[*i], "--preload") == 0)
		ctx->flags |= FLAG_VERBOSE;
	else if (strcmp(argv[*i], "-s") == 0 || strcmp(argv[*i], "--preload") == 0)
		ctx->flags |= FLAG_VERBOSE;
	else
		return (1);
	return (0);
}

int parse_args(t_ping_ctx *ctx, int argc, char **argv)
{
	int i;

	ctx->progname = argv[0];
	i = 1;
	while (i < argc) {
		if (argv[i][0] == '-') {
			if (parse_flag(ctx, argc, argv, &i) != 0)
				return (1);
		}
		else
			if (parse_hostname(ctx, argv[i]) != 0)
				return (1);
		i++;
	}
	if (!(ctx->flags & FLAG_HELP) && !ctx->hostname) {
		dprintf(2, "%s: Error: Missing hostname\n", ctx->progname);
		print_help(ctx->progname);
		return (1);
	}
	return (0);
}
