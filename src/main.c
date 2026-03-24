#include "ping.h"

#include <stdio.h>  // fprintf()
#include <sys/socket.h>  // socket()

//https://www.geeksforgeeks.org/computer-networks/ping-in-c/

#include "ping.h"


int is_ip(char *str)
{
	struct sockaddr_in sa;
}

void ping_ctx_init(s_ping_ctx *ping_ctx, int argc, char **argv)
{

}

int main(int argc, char **argv)
{
	s_ping_ctx ping_ctx;

	if (argc < 2) {
		return (1);
	}

	ping_ctx_init(argc, argv);

	ping(argc, argv);

	return (0);
}
