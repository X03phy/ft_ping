#include "ping.h"

#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socklen_t, struct sockaddr, sendto(), recvfrom()
#include <sys/time.h> // struct timeval, gettimeofday()
#include <sys/types.h> // ssize_t

#include <stddef.h> // size_t
#include <stdio.h> // dprintf(), perror()
#include <unistd.h>

int icmp_send(const t_icmp_pkt *pkt, const t_ping_ctx *ctx)
{
	ssize_t ret;

	ret = sendto(ctx->sockfd, pkt, sizeof(*pkt), 0,
	             (struct sockaddr *)&ctx->addr, sizeof(ctx->addr));
	if (ret == -1) {
		perror("sendto()");
		return (1);
	}
	if ((size_t)ret != sizeof(*pkt)) {
		dprintf(2, "partial send\n"); //! Modify error message
		return (1);
	}
	return (0);
}

static int icmp_parse(t_icmp_pkt *pkt, const char *buf, size_t len)
{
	struct ip *ip;
	t_icmp_pkt *recv_pkt;
	size_t ip_hlen;

	if (len < IP_HDR_SIZE + sizeof(t_icmp_pkt)) {
		dprintf(2, "icmp_parse(): packet too short\n");
		return (1);
	}
	ip = (struct ip *)buf;
	ip_hlen = ip->ip_hl * 4;
	recv_pkt = (t_icmp_pkt *)(buf + ip_hlen);
	if (recv_pkt->hdr.type != ICMP_ECHOREPLY) {
		dprintf(2, "icmp_parse(): not an echo reply (type=%d)\n",
			recv_pkt->hdr.type);
		return (1);
	}
	if (ntohs(recv_pkt->hdr.id) != (uint16_t)(getpid() & 0xFFFF))
		return (1);
	*pkt = *recv_pkt;
	pkt->hdr.seq = ntohs(recv_pkt->hdr.seq);
	pkt->hdr.id = ntohs(recv_pkt->hdr.id);
	pkt->hdr.checksum = ntohs(recv_pkt->hdr.checksum);
	return (0);
}

int icmp_recv(t_icmp_pkt *pkt, t_ping_ctx *ctx)
{
	char buf[IP_HDR_SIZE + sizeof(t_icmp_pkt)];
	struct sockaddr_in from;
	socklen_t fromlen;
	ssize_t r;

	fromlen = sizeof(from);
	r = recvfrom(ctx->sockfd, buf, sizeof(buf), 0,
		     (struct sockaddr *)&from,
	             &fromlen);
	//gettimeofday(tv_recv, NULL);
	if (r == -1) {
		perror("recvfrom()");
		return (1);
	}
	return (icmp_parse(pkt, buf, (size_t)r));
}
