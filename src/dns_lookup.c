//#include "ping.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *dns_lookup(const char *hostname)
{
	int s;
	char *ip;
	struct addrinfo hints;
	struct addrinfo *res;
	struct sockaddr_in *addr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	s = getaddrinfo(hostname, NULL, &hints, &res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(s));
		return (NULL);
	}

	addr = (struct sockaddr_in *)res->ai_addr;

	ip = malloc(INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);

	freeaddrinfo(res);
	return (ip);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		return (1);

	char ip[INET_ADDRSTRLEN];
	printf("%s\n", dns_lookup(argv[1], ip));
	return (0);
}
