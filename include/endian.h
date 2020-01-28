/**
 * @file endian.c
 * @author Philip R. Simonson
 * @date 01/28/2020
 * @brief Check for big/little endian at runtime.
 ***********************************************************************
 */

#ifndef _PRS_ENDIAN_H_
#define _PRS_ENDIAN_H_

/** @brief Enumeration for endianess */
enum _prs_endian {
	LITTLE_ENDIAN = 0x03020100ul,	/**< LITTLE_ENDIAN */
	BIG_ENDIAN = 0x00010203ul,		/**< BIG_ENDIAN */
	PDP_ENDIAN = 0x01000302ul,		/**< PDP-11 */
	HONEYWELL_ENDIAN = 0x02030001ul /**< honeywell */
};

/** @brief Test for endianess, returns enumeration. */
int check_endian(void);

#endif
