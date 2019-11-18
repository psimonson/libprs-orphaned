/**
 * @file bitfiddle.h
 * @author Philip R. Simonson
 * @date 10/28/2019
 * @brief Bit fiddling header file.
 *******************************************************
 * @details
 *
 * Simple bit fiddling, modifying bits for an integer.
 *******************************************************
 */

#ifndef _BITFIDDLE_H_
#define _BITFIDDLE_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Get a bit from Nth position. */
PRS_EXPORT int get_bit(int, int);
/** @brief Set a bit at Nth position. */
PRS_EXPORT int set_bit(int, int);
/** @brief Clear a bit at Nth position. */
PRS_EXPORT int clr_bit(int, int);

#define GET_BIT(a,b) get_bit(a, b) /** @brief Same as above. */
#define SET_BIT(a,b) set_bit(a, b) /** @brief Same as above. */
#define CLR_BIT(a,b) clr_bit(a, b) /** @brief Same as above. */

#ifdef __cplusplus
}
#endif

#endif
