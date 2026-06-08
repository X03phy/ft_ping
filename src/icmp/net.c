#include "ping.h"

#include <arpa/inet.h> // ntohs()
#include <netinet/ip_icmp.h> // struct ip, ICMP_ECHOREPLY, IP_MAXPACKET
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
	size_t ip_hlen;
	t_icmp_hdr *icmp;

	if (len < sizeof(struct ip) + sizeof(t_icmp_hdr))
		return (1);
	ip = (struct ip *)buf;
	ip_hlen = ip->ip_hl * 4;
	icmp = (t_icmp_hdr *)(buf + ip_hlen);
	//if (recv_pkt->hdr.type != ICMP_ECHOREPLY ||
	//    ntohs(recv_pkt->hdr.id) != (uint16_t)(getpid() & 0xFFFF))
	//	return (1);
	reply->ip_hdr = *ip;
	reply->type = icmp->type;
	reply->code = icmp->code;
	reply->seq = icmp->seq;
	reply->ttl = ip->ip_ttl;
	printf("%d\n", reply->ttl);

	if (icmp->type == ICMP_ECHOREPLY) {
		if (ntohs(icmp->id) != (uint16_t)(getpid() & 0xFFFF))
			return (1);
		reply->pkt = *(t_icmp_pkt *)(buf + ip_hlen);
		reply->ip_hdr = *ip;
		//reply->pkt.hdr.seq = ntohs(recv_pkt->hdr.seq);
		//reply->pkt.hdr.id = ntohs(recv_pkt->hdr.id);
		reply->ttl = ip->ip_ttl;
	}
	else
		printf("Hello\n");
	return (0);
}

int icmp_recv(t_ping_ctx *ctx, t_icmp_reply *reply)
{
	char buf[IP_MAXPACKET];
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
