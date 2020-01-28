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
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

/** @brief Test for endianess, returns enumeration. */
int check_endian(void);

#endif
