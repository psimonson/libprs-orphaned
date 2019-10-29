/*
 * bitfiddle.c - Simple bit fiddling source.
 * Author: Philip R. Simonson
 * Date  : 10/28/2019
 ************************************************************
 */

#include "bitfiddle.h"

int get_bit(int byte, int bit)
{
	return (byte & (1 << bit));
}

int set_bit(int byte, int bit)
{
	return (byte | (1 << bit));
}

int clr_bit(int byte, int bit)
{
	return (byte & ~(1 << bit));
}
