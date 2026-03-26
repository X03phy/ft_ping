#ifndef PING_H
#define PING_H

/* Includes */
#include <arpa/inet.h> // INET_ADDRSTRLEN
#include <stddef.h>    // size_t

/* Macros */
/* Messages */
#define PING_USAGE_FORMAT "Usage: %s [flags] host\n"
#define PING_UNKNOWN_HOST_FORMAT "%s: Error: Cannot resolve %s: Unknown host"

/* Default values */
#define PING_DEFAULT_TTL      64
#define PING_DEFAULT_COUNT    0
#define PING_DEFAULT_INTERVAL 1.0
#define PING_DEFAULT_TIMEOUT  1.0
#define PING_DEFAULT_PAYLOAD  56

/* Structures */
typedef struct {
	const char *progname;
	const char *target;

	char       ip[INET_ADDRSTRLEN];

	int    ttl;          // -t : Time To Live
	int    count;        // -c : nombre de paquets
	double interval;     // -i : intervalle entre envois (secondes)
	double timeout;      // -W : timeout par réponse
	int    verbose;      // -v : mode verbeux
	size_t packet_size;  // -s : taille du payload

	unsigned long flags;

	//uint16_t pid;        // identifiant ICMP = getpid()
	//uint16_t seq;        // numéro de séquence courant

	//struct sockaddr_in dest;     // adresse destination résolue

} s_ping_ctx;

/* Functions */
/* print.c */

/* dns_lookup.c */
int dns_lookup(char ip[INET_ADDRSTRLEN], const char *target);

/* ping_ctx.c */
void ping_ctx_init(s_ping_ctx *ctx);
int ping_ctx_setup(s_ping_ctx *ctx);

/* ping.c */
int ping(s_ping_ctx *ctx);

#endif
