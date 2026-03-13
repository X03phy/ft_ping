#include <stdio.h>  // fprintf()
#include <sys/socket.h>  // socket()

//https://www.geeksforgeeks.org/computer-networks/ping-in-c/


/*
 * Functions
 */

int main(int argc, char **argv)
{
	int sockfd;

	if (argc != 2) {
		fprintf(stderr, "");
		return (1);
	}

	ip_addr

	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)
	if (sockfd < 0) {
		perror("socket() failed");
		return (1);
	}

	return (0);
}

