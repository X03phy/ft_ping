#include "ping.h"

#include <ctype.h> // isxdigit()
#include <limits.h> // INT_MAX
#include <stddef.h> // NULL()
#include <stdio.h> // dprintf()
#include <string.h> // strcmp()
#include <stdlib.h> // strol()

typedef int (*t_parser)(const char *str, void *out);

static int parse_hostname(t_ping_ctx *ctx, char *arg)
{
	if (!ctx->hostname) {
		ctx->hostname = arg;
		return (0);
	}
	dprintf(2, "%s: Error: Too many hostnames\n", ctx->progname);
	return (1);
}

static int parse_value_int(const char *str, void *out)
{
	char *end;
	long val;

	val = strtol(str, &end, 10);
	if (*end != '\0' || val <= 0 || val > INT_MAX)
		return (1);
	*(int *)out = (int)val;
	return (0);
}

static int parse_value_double(const char *str, void *out)
{
	char *end;
	double val;

	val = strtod(str, &end);
	if (*end != '\0' || val <= 0)
		return (1);
	*(double *)out = (double)val;
	return (0);
}

static int parse_value_pattern(const char *str, void *out)
{
	if (strlen(str) != 2)
		return (1);
	if (!isxdigit((unsigned char)str[0])
	    || !isxdigit((unsigned char)str[1]))
		return (1);
	*(unsigned char *)out = (unsigned char)strtol(str, NULL, 16);
	return (0);
}

static int parse_value(t_ping_ctx *ctx, int argc, char **argv,
                       int *i, t_parser parser, void *out)
{
	if (*i + 1 == argc) {
		dprintf(2, "%s: Error: Option '%s' requires a value\n",
		        ctx->progname, argv[*i]);
		return (1);
	}
	if (parser(argv[*i + 1], out)) {
		dprintf(2, "%s: Error: Option %s: Value '%s' is invalid\n",
		        ctx->progname, argv[*i], argv[*i + 1]);
		return (1);
	}
	(*i)++;
	return (0);
}

static int parse_flag(t_ping_ctx *ctx, int argc, char **argv, int *i)
{
	if (strcmp(argv[*i], "-?") == 0
	           || strcmp(argv[*i], "--help") == 0)
		ctx->flags |= FLAG_HELP;
	else if (strcmp(argv[*i], "-c") == 0
	        || strcmp(argv[*i], "--count") == 0) {
		if (parse_value(ctx, argc, argv, i,
		                parse_value_int, &ctx->count) != 0)
			return (1);
	}
	else if (strcmp(argv[*i], "-i") == 0
	         || strcmp(argv[*i], "--interval") == 0) {
		if (parse_value(ctx, argc, argv, i,
		                parse_value_double, &ctx->interval) != 0)
			return (1);
	}
	else if (strcmp(argv[*i], "-v") == 0
	         || strcmp(argv[*i], "--verbose") == 0)
		ctx->flags |= FLAG_VERBOSE;
	else if (strcmp(argv[*i], "-f") == 0
	         || strcmp(argv[*i], "--flood") == 0) {
		ctx->flags |= FLAG_FLOOD;
		ctx->interval = 0.01;
	}
	else if (strcmp(argv[*i], "-p") == 0
	        || strcmp(argv[*i], "--pattern") == 0) {
		if (parse_value(ctx, argc, argv, i,
		                parse_value_pattern, &ctx->pattern) != 0)
			return (1);
	}
	else if (strcmp(argv[*i], "-q") == 0
	         || strcmp(argv[*i], "--quiet") == 0)
		ctx->flags |= FLAG_QUIET;
	else {
		dprintf(2, "%s: Error: Option '%s' is invalid\n",
		        ctx->progname, argv[*i]);
		return (1);
	}
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
