#include "ping.h"

#include <arpa/inet.h> // htons()
#include <sys/time.h> // struct timeval, gettimeofday()

#include <unistd.h> // getpid()
#include <stddef.h> // NULL, size_t
#include <string.h> // memcpy(), memset()

static void icmp_init_hdr(s_icmp_pkt *pkt, unsigned short seq);
static void icmp_fill_data_with_time(s_icmp_pkt *pkt);
static unsigned short icmp_checksum(void *data, size_t len);

void icmp_build(s_icmp_pkt *pkt, unsigned short seq)
{
	memset(pkt, 0, sizeof(*pkt));
	icmp_init_hdr(pkt, seq);
	icmp_fill_data_with_time(pkt);
	pkt->hdr.checksum = icmp_checksum(pkt, sizeof(*pkt));
}

static void icmp_init_hdr(s_icmp_pkt *pkt, unsigned short seq)
{
	pkt->hdr.type = ICMP_ECHO;
	pkt->hdr.code = 0;
	pkt->hdr.id = htons((unsigned short)getpid());
	pkt->hdr.seq = htons(seq);
}

static void icmp_fill_data_with_time(s_icmp_pkt *pkt)
{
	struct timeval tv;

	gettimeofday(&tv, NULL); /* Cannot fail if used properly */
	memcpy(pkt->data, &tv, sizeof(tv));
	memset(pkt->data + sizeof(tv), 0, DATA_SIZE - sizeof(tv));
}

static unsigned short icmp_checksum(void *data, size_t len) //! recheck
{
	unsigned short *buf;
	unsigned int   sum;

	buf = data;
	sum = 0;
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
