// #include "ping.h"

#include <string.h>     // memset()
#include <netdb.h>  // struct addrinfo, struct sockaddr_in
#include <arpa/inet.h>
#include <stdio.h>      // perror()
#include <stdlib.h>     // malloc(), free()

int dns_lookup(char ip[INET_ADDRSTRLEN], const char *target)
{
	struct addrinfo    hints;
	int                errcode;
	struct addrinfo    *res;
	struct sockaddr_in *addr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	errcode = getaddrinfo(target, NULL, &hints, &res);
	if (errcode != 0) {
		fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(errcode));
		return (1);
	}

	addr = (struct sockaddr_in *)res->ai_addr;

	if (!inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN)) {
		perror("inet_ntop()");
		errcode = 1;
	}

	freeaddrinfo(res);
	return (errcode);
}
