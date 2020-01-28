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
	unsigned short var = 0x1000;
	return (var & 1) ? LITTLE_ENDIAN : BIG_ENDIAN;
}
