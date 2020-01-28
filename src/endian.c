/**
 * @file endian.c
 * @author Philip R. Simonson
 * @date 01/28/2020
 * @brief Check for big/little endian on system at runtime.
 ***********************************************************************
 */

#include "endian.h"

/* Checks for endianess at runtime.
 */
int check_endian(void)
{
	static const union {
		unsigned char bytes[4];
		unsigned long value;
	} _host_order = {
		{ 0, 1, 2, 3 }
	};
	return _host_order.value;
}
