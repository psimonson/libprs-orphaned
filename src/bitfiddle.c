/*
 * bitfiddle.c - Simple bit fiddling source.
 * Author: Philip R. Simonson
 * Date  : 10/28/2019
 ************************************************************
 */

#include "bitfiddle.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Get a bit from a byte.
 */
int get_bit(int byte, int bit)
{
	return (byte & (1 << bit));
}
/* Set a bit to a byte.
 */
int set_bit(int byte, int bit)
{
	return (byte | (1 << bit));
}
/* Clear a bit on a byte.
 */
int clr_bit(int byte, int bit)
{
	return (byte & ~(1 << bit));
}
#ifdef __cplusplus
}
#endif
