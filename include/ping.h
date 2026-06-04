#ifndef PING_H
#define PING_H

/* Includes */
#include <netinet/in.h> // struct sockaddr_in

#include <sys/time.h> // struct timeval

#include <stdint.h> // uintX_t

/* Macros */
#define DATA_SIZE 56

#define FLAG_VERBOSE (1 << 0) // -v, --verbose
#define FLAG_HELP (1 << 1) // -h, -?, --help
#define FLAG_FLOOD (1 << 2) // -f
#define FLAG_PRELOAD (1 << 3) // -l
#define FLAG_NUMERIC (1 << 4) // -n
#define FLAG_DEADLINE (1 << 5) // -w
#define FLAG_TIMEOUT (1 << 4) // -W
#define FLAG_PATTERN (1 << 0) // -p, --pattern
#define FLAG_IGNORE_ROUTING (1 << 0) // -p, --pattern

#define DEFAULT_INTERVAL 1.0

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
	int timeout;

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

extern int g_pingloop;

/* Prototypes */
/* icmp/build.c */
t_icmp_pkt icmp_build(unsigned short seq);

/* icmp/net.c */
int icmp_send(const t_ping_ctx *ctx, const t_icmp_pkt *pkt);
int icmp_recv(t_ping_ctx *ctx, t_icmp_reply *reply);

/* ping/init.c */
void ping_init(t_ping_ctx *ctx);

/* ping/setup.c */
int ping_setup(t_ping_ctx *ctx);

/* ping/run.c */
int ping_run(t_ping_ctx *ctx);

/* ping/cleanup.c */
void ping_cleanup(t_ping_ctx *ctx);

/* signal/setup.c */
int signal_setup(void);

/* time/diff.c */
double time_diff_ms(struct timeval *start, struct timeval *end);

/* utils/args.c */
int parse_args(t_ping_ctx *ctx, int argc, char **argv);

/* utils/print.c */
void print_help(const char *progname);
void print_header(const t_ping_ctx *ctx);
void print_response(const t_ping_ctx *ctx, const t_icmp_reply *reply,
                    double rtt);
void print_stats(const t_ping_ctx *ctx);

#endif
