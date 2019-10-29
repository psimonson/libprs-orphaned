/*
 * bitfiddle.h - Simple bit fiddling.
 * Author: Philip R. Simonson
 * Date  : 10/28/2019
 *******************************************************
 */

#ifndef _BITFIDDLE_H_
#define _BITFIDDLE_H_

#include "export.h"

PRS_EXPORT int get_bit(int, int);
PRS_EXPORT int set_bit(int, int);
PRS_EXPORT int clr_bit(int, int);

#define GET_BIT(a,b) get_bit(a, b)
#define SET_BIT(a,b) set_bit(a, b)
#define CLR_BIT(a,b) clr_bit(a, b)

#endif
