#include "ping.h"

#include <unistd.h> // close()

void ping_cleanup(t_ping_ctx *ctx)
{
	if (ctx->sockfd != -1)
		close(ctx->sockfd);	
}
