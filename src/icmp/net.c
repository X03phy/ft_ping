#include "ping.h"

#include <arpa/inet.h> // ntohs()
#include <netinet/ip_icmp.h> // struct ip, ICMP_ECHOREPLY
#include <sys/socket.h> // struct sockaddr, recvfrom(), sendto()

#include <sys/time.h> // gettimeofday()
#include <sys/types.h> // ssize_t
#include <unistd.h> // getpid()

#include <errno.h> // EAGAIN, EINTR, EWOULDBLOCK, errno
#include <stddef.h> // size_t, NULL
#include <stdint.h> // uintX_t
#include <stdio.h> // perror()

int icmp_send(const t_ping_ctx *ctx, const t_icmp_pkt *pkt)
{
	ssize_t ret;

	ret = sendto(ctx->sockfd, pkt, sizeof(*pkt), 0,
	             (struct sockaddr *)&ctx->addr, sizeof(ctx->addr));
	if (ret == -1 || (size_t)ret != sizeof(*pkt)) {
		perror("sendto()");
		return (1);
	}
	return (0);
}

static int icmp_parse(t_icmp_reply *reply, const char *buf, size_t len) //! Add expected seq
{
	struct ip *ip;
	t_icmp_pkt *recv_pkt;

	if (len < sizeof(struct ip) + sizeof(t_icmp_pkt))
		return (1);
	ip = (struct ip *)buf;
	recv_pkt = (t_icmp_pkt *)(buf + ip->ip_hl * 4);
	if (recv_pkt->hdr.type != ICMP_ECHOREPLY ||
	    ntohs(recv_pkt->hdr.id) != (uint16_t)(getpid() & 0xFFFF))
		return (1);
	reply->pkt = *recv_pkt;
	reply->pkt.hdr.seq = ntohs(recv_pkt->hdr.seq);
	reply->pkt.hdr.id = ntohs(recv_pkt->hdr.id);
	reply->ttl = ip->ip_ttl;
	return (0);
}

int icmp_recv(t_ping_ctx *ctx, t_icmp_reply *reply)
{
	char buf[sizeof(struct ip) + sizeof(t_icmp_pkt)];
	socklen_t fromlen;
	ssize_t r;

	while (1) {
		fromlen = sizeof(reply->from);
		r = recvfrom(ctx->sockfd, buf, sizeof(buf), 0,
		             (struct sockaddr *)&reply->from, &fromlen);
		gettimeofday(&reply->tv_recv, NULL);
		if (r == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK
			    || errno == EINTR)
				return (1);
			perror("recvfrom()");
			return (1);
		}
		if (icmp_parse(reply, buf, (size_t)r) == 0)
			return (0);
	}
}
