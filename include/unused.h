/**
 * @file unused.h
 * @author Philip R. Simonson
 * @date 06/14/2019
 * @brief Simple macros for unused parameters/functions/variables.
 *************************************************************************
 */

#ifndef PRS_UNUSED_H
#define PRS_UNUSED_H

/** @brief Unused - Function parameter or variable not used. */
#ifndef UNUSED
#ifdef __GNUC__
#define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
#define UNUSED(x) UNUSED_ ## x
#endif
#endif

/** @brief Unused function - Function isn't used. */
#ifndef UNUSED_FUNCTION
#ifdef __GNUC__
#define UNUSED_FUNCTION(x) __attribute__((unused)) UNUSED_ ## x
#else
#define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif
#endif

#endif
