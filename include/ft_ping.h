#pragma once

#include <stdint.h>  // For types

typedef struct s_icmp {
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint8_t data[];
} t_icmp;

t_icmp *mkicmp(uint8_t, uint8_t, uint8_t *, uint16_t);

int main(int, char **);