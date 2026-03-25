#include "ping.h"

int main(int argc, char **argv)
{
	s_ping_ctx ping_ctx;

	if (argc < 2) {
		return (1);
	}

	ping_ctx_init(&ping_ctx);

	ping_ctx_setup(&ping_ctx, argc, argv);

	ping(argc, argv);

	return (0);
}
