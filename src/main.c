#include "ping.h"

#include <stdio.h>

int main(int argc, char **argv)
{
	s_ping_ctx ctx;

	if (argc < 2) {
		fprintf(stderr, PING_USAGE_FORMAT, argv[0]);
		return (1);
	}

	ping_ctx_init(&ctx);

	if (parse_args(&ctx, argc, argv) != 0)
		return (1);

	if (ping_ctx_setup(&ctx) != 0)
		return (1);

	ping(&ctx);

	// ping_ctx_cleanup(&ctx);
	return (0);
}
