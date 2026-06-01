#include "ping.h"

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
