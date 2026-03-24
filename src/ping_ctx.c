#include "ping.h"

#include <string.h> // memset()

void ping_ctx_init(s_ping_ctx *ping_ctx)
{
	memset(ping_ctx, 0, sizeof(*ping_ctx));
}

int ping_ctx_setup(s_ping_ctx *ping_ctx, int argc, char **argv)
{
	
}
