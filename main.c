#include <stdio.h>
#include <netinet/ip_icmp.h> // ICMP_ECHO
int main()
{
	int hlen = ip->ip_hl << 2;
	printf("%d\n", hlen);
	return (0);
}