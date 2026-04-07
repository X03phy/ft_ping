#include "ping.h"

#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socklen_t, struct sockaddr, sendto(), recvfrom()
#include <sys/types.h>  // ssize_t

#include <stddef.h> // size_t
#include <stdio.h>  // dprintf(), perror()

int icmp_send(s_icmp_pkt *pkt, int sockfd, struct sockaddr_in *addr)
{
	ssize_t ret;

	ret = sendto(sockfd, pkt, sizeof(*pkt), 0, (struct sockaddr *)addr, sizeof(*addr));
	if (ret == -1) {
		perror("sendto()");
		return (1);
	}
	if ((size_t)ret != sizeof(*pkt)) {
		dprintf(2, "partial send\n");
		return (1);
	}

	return (0);
}

int icmp_recv(size_t *out, int sockfd, char *buf, size_t len, struct sockaddr_in *from)
{
	socklen_t fromlen;
	ssize_t   r;

	fromlen = sizeof(*from);
	r = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)from, &fromlen);
	if (r == -1) {
		perror("recvfrom()");
		return (1);
	}

	*out = (size_t)r;
	return (0);
}
