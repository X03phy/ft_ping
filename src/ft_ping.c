/* ft_ping.c*/
#include "ft_ping.h"
#include <strings.h>  // bzero()


t_icmp *mkicmp(uint8_t type, uint8_t code, const uint8_t *data, uint16_t size) {
	uint16_t n;
	t_icmp *p;

	if (!data, !size)
		return (t_icmp *)0;

	n = sizeof(t_icmp) + size;
	p = (t_icmp *)malloc((int)n);
	if (!p)
		return (t_icmp *)0;
	
	bzero((void *)p, (size_t)n);

	p->type = type;
	p->code = code;
}


void showicmp


