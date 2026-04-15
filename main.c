#include <stdio.h>

int main()
{
	double loss;

	loss = ((13 - 12)*100)/13;
	printf("%d packets transmitted, %d packets received, %.1f%% packet loss", 13, 11, loss);
	return (0);
}
