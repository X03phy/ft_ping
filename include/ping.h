#ifndef PING_H
#define PING_H

/* Includes */

/* Macros */
#define PING_USAGE_FORMAT "Usage: %s [flags] host\n"


/* Structures */
typedef struct {
	const char    *progname;
	const char    *hostname;
	const char    *ip_addr;
	unsigned long flag;
} s_ping_ctx;

/* Functions */
int ping(int argc, char **argv);

#endif
