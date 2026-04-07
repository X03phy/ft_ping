#include "ping.h"

#include <stdio.h> // printf()

void print_help(const char *progname)
{
	printf("Usage:\n");
	printf("  %s [options] host\n", progname);
	printf("\n");
	printf("Options:\n");
	printf("  -v\n");
	printf("\n");

}

void print_error(int errcode, const char *progname)
{
	(void)errcode;
	(void)progname;
}
