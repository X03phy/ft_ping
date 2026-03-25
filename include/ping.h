#ifndef PING_H
#define PING_H

/* Includes */
#include <arpa/inet.h> // INET_ADDRSTRLEN

/* Macros */
#define PING_USAGE_FORMAT "Usage: %s [flags] host\n"

/* Structures */
typedef struct {
	const char    *progname;
	const char    *target;
	unsigned long flags;
	unsigned int count;
	double interval;
	int ttl;
} s_ping_args;

typedef struct {
	s_ping_args   ping_args;
	char    *hostname;
	char    ip[INET_ADDRSTRLEN];
} s_ping_ctx;

/* Functions */
/* dns_lookup.c */
int dns_lookup(char ip[INET_ADDRSTRLEN], const char *hostname);

/* ping_ctx.c */
void ping_ctx_init(s_ping_ctx *ping_ctx);
int ping_ctx_setup(s_ping_ctx *ping_ctx, int argc, char **argv);


int ping(int argc, char **argv);

#endif
