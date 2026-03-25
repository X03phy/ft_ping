#include "ping.h"

#include <string.h> // memset()
#include <stdio.h>
void ping_ctx_init(s_ping_ctx *ping_ctx)
{
	memset(ping_ctx, 0, sizeof(*ping_ctx));
}

int ping_ctx_setup(s_ping_ctx *ping_ctx, int argc, char **argv)
{
	(void)argc;
	ping_ctx->ping_args.progname = argv[0];

	// ping_parse_args();
	ping_ctx->ping_args.target = argv[1];

	if (dns_lookup(ping_ctx->ip, ping_ctx->ping_args.target) != 0) {
		fprintf(stderr, "yess\n");
	}

	return (0);
}
