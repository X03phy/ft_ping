#include "ping.h"

#include <string.h> // memset()
#include <stdio.h>

void ping_ctx_init(s_ping_ctx *ping_ctx)
{
	memset(ping_ctx, 0, sizeof(s_ping_ctx));

	/* Valeurs par défaut des options */
	ping_ctx->opts.ttl         = PING_DEFAULT_TTL;
	ping_ctx->opts.count       = PING_DEFAULT_COUNT;
	ping_ctx->opts.interval    = PING_DEFAULT_INTERVAL;
	ping_ctx->opts.timeout     = PING_DEFAULT_TIMEOUT;
	ping_ctx->opts.packet_size = PING_DEFAULT_PAYLOAD;

	/* État initial */
	ping_ctx->sockfd = -1;
	//ping_ctx->seq    = 0;
//	ping_ctx->pid    = (uint16_t)getpid();
//	ping_ctx->flags  |= FLAG_RUNNING;
}

int ping_ctx_setup(s_ping_ctx *ping_ctx, int argc, char **argv)
{
	(void)argc;
	ping_ctx->progname = argv[0];

	// ping_parse_args();
	ping_ctx->target = argv[1];

	if (dns_lookup(ping_ctx->ip, ping_ctx->target) != 0) {
		fprintf(stderr, PING_UNKNOWN_HOST_FORMAT, ping_ctx->progname,
			ping_ctx->target);
		return (1);
	}

	return (0);
}
