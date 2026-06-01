#include "ping.h"

static void sig_handler(int dummy)
{
	(void)dummy;
	g_pingloop = 0;
}
