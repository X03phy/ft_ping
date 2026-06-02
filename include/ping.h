#ifndef PING_H
#define PING_H

/* Includes */
#include <netinet/in.h> // struct sockaddr_in
#include <sys/time.h> // struct timeval

#include <stdint.h> // uintX_t

#define DATA_SIZE 56

/* Structures */
typedef struct t_icmp_hdr {
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t id;
	uint16_t seq;
} t_icmp_hdr;

typedef struct t_icmp_pkt {
	t_icmp_hdr hdr;
	char data[DATA_SIZE];
} t_icmp_pkt;

typedef struct s_icmp_reply {
    t_icmp_pkt pkt;
    struct sockaddr_in from;
    int ttl;
    struct timeval tv_recv;
} t_icmp_reply;

typedef struct t_ping_ctx {
	/* Args */
	const char *progname;
	const char *hostname;

	int ttl;
	long long count;
	double interval;
	unsigned long flags;

	/* Socket */
	int sockfd;
	struct sockaddr_in addr;

	/* Stats */
	unsigned int send;
	unsigned int received;
	double rtt_min;
	double rtt_max;
	double rtt_sum;
	double rtt_sum_sq;
} t_ping_ctx;

/* Macros */
#define FLAG_VERBOSE (1 << 0)

#define DEFAULT_INTERVAL 1.0
// #define PING_DEFAULT

#define DATA_SIZE 56
#define IP_HDR_SIZE 20

/* Functions */
/* error.c */
void print_error(int errcode, const char *progname);
void print_help(const char *progname);

/* args.c */
int parse_args(t_ping_ctx *ctx, int argc, char **argv);

/* icmp/icmp.c */
t_icmp_pkt icmp_build(unsigned short seq);

/* icmp/icmp_io.c */
int icmp_send(const t_ping_ctx *ctx, const t_icmp_pkt *pkt);
int icmp_recv(t_ping_ctx *ctx, t_icmp_reply *reply);

/* stats.c */
void print_header(const t_ping_ctx *ctx);
void print_response(const t_icmp_reply *reply, double rtt);
void print_stats(const t_ping_ctx *ctx);


/* ping/init.c */
void ping_cleanup(t_ping_ctx *ctx);
void ping_init(t_ping_ctx *ctx);
int ping_run(t_ping_ctx *ctx);
int ping_setup(t_ping_ctx *ctx);

/* time/time.c */
double time_diff_ms(struct timeval *start, struct timeval *end);

#endif
