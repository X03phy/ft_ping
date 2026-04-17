#ifndef PING_H
#define PING_H

/* Includes */
#include <netinet/in.h> // struct sockaddr_in
#include <netinet/ip_icmp.h> // struct icmp

/* Macros */
#define PING_FLAG_VERBOSE (1 << 0)

#define PING_DEFAULT_INTERVAL 1.0
// #define PING_DEFAULT

#define DATA_SIZE 56
#define IP_HDR_SIZE 20

/* Structures */
typedef struct s_icmp_pkt {
	struct icmp hdr;
	char data[DATA_SIZE];
} s_icmp_pkt;

typedef struct s_ping_ctx {
	/* Args */
	const char *progname;
	const char *host;

	long long count;
	double interval;
	unsigned long flags;

	/* Socket */
	int sockfd;
	struct sockaddr_in addr;

	/* Stats */
	unsigned int sent;
	unsigned int received;
	double rtt_min;
	double rtt_max;
	double rtt_sum;
	double rtt_total;
} s_ping_ctx;

/* Functions */
/* error.c */
void print_error(int errcode, const char *progname);
void print_help(const char *progname);

/* args.c */
int parse_args(s_ping_ctx *ctx, int argc, char **argv);

/* net.c */
int ping_setup(s_ping_ctx *ctx);

/* icmp/icmp.c */
void icmp_build(s_icmp_pkt *pkt, unsigned short seq);

/* icmp/icmp_io.c */
int icmp_send(s_icmp_pkt *pkt, int sockfd, struct sockaddr_in *addr);
int icmp_recv(size_t *out, int sockfd, char *buf, size_t len, struct sockaddr_in *from);

/* stats.c */
void print_header(const s_ping_ctx *ctx);

void print_stats(const s_ping_ctx *ctx);


/* ping.c */
int ping_run(s_ping_ctx *ctx);

#endif
