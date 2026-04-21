#include "ping.h"

#include <arpa/inet.h> // htons()
#include <sys/time.h> // struct timeval, gettimeofday()

#include <unistd.h> // getpid()
#include <stddef.h> // NULL, size_t
#include <string.h> // memcpy(), memset()

static void icmp_init_hdr(s_icmp_pkt *pkt, unsigned short seq);
static void icmp_fill_data(s_icmp_pkt *pkt);
static unsigned short icmp_checksum(void *data, size_t len);

void icmp_build(s_icmp_pkt *pkt, unsigned short seq)
{
	memset(pkt, 0, sizeof(*pkt));
	icmp_init_hdr(pkt, seq);
	icmp_fill_data(pkt);
	pkt->hdr.icmp_cksum = icmp_checksum(pkt, sizeof(struct icmp) + DATA_SIZE);
}

static void icmp_init_hdr(s_icmp_pkt *pkt, unsigned short seq)
{
	pkt->hdr.icmp_type = ICMP_ECHO;
	pkt->hdr.icmp_code = 0;
	pkt->hdr.icmp_id = htons((unsigned short)getpid());
	pkt->hdr.icmp_seq = htons(seq);
}

static void icmp_fill_data(s_icmp_pkt *pkt)
{
	struct timeval tv;

	gettimeofday(&tv, NULL); /* Cannot fail if used properly */
	memcpy(pkt->data, &tv, sizeof(tv));
	memset(pkt->data + sizeof(tv), 0, DATA_SIZE - sizeof(tv));
}

static unsigned short icmp_checksum(void *data, size_t len) //! recheck
{
	unsigned short *buf = data;
	unsigned int   sum = 0;

	while (len > 1) {
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return (~sum);
}
