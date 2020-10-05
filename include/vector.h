/**
 * @file vector.h
 * @author Philip R. Simonson
 * @date 21 June 2019
 * @brief Simple C++ vector implementation in pure C.
 *********************************************************************
 * @details
 *
 * This is a simple C++ vector implementation written in pure
 * C. Has just about everything you'd need in a vector.
 */

#ifndef PRS_VECTOR_H
#define PRS_VECTOR_H

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#define vector_set_capacity(vec, size) ((size_t*)(vec))[-1] = (size)
#define vector_set_size(vec, size) ((size_t*)(vec))[-2] = (size)

/** @brief Get the total capacity of the vector. (Internal use ONLY) */
#define vector_capacity(vec) ((vec) ? ((size_t*)(vec))[-1] : (size_t)0)
/** @brief Get the size of the vector. */
#define vector_size(vec) ((vec) ? ((size_t*)(vec))[-2] : (size_t)0)
/** @brief Get the whether the vector is empty. */
#define vector_empty(vec) (vector_size(vec) == 0)

/** @brief
 * Grow the vector in capacity. (Internal use ONLY)
 */
#define vector_grow(vec, count) do { \
	if(!(vec)) { \
		size_t *_p; \
		_p = malloc((count)*sizeof(*(vec))+(sizeof(size_t)*2)); \
		assert(_p); \
		(vec) = (void*)(&_p[2]); \
		vector_set_capacity((vec), (count)); \
		vector_set_size((vec), 0); \
	} else { \
		size_t *_p1, *_p2; \
		_p1 = &((size_t*)(vec))[-2]; \
		_p2 = \
		realloc(_p1, (count)*sizeof(*(vec))+(sizeof(size_t)*2)); \
		assert(_p2); \
		(vec) = (void*)(&_p2[2]); \
		vector_set_capacity((vec), (count)); \
	} \
} while(0)

/** @brief Delete an element at i position. */
#define vector_erase(vec, i) do { \
	if(vec) { \
		const size_t __sz = vector_size(vec); \
		if((i) < __sz) { \
			size_t __x; \
			vector_set_size((vec), __sz-1); \
			for(__x = (i); __x < (__sz-1); ++__x) { \
				(vec)[__x] = (vec)[__x+1]; \
			} \
		} \
	} \
} while(0)

/** @brief Release resources of a vector. */
#define vector_free(vec) do { \
	if(vec) { \
		size_t *p1 = &((size_t*)(vec))[-2]; \
		free(p1); \
	} \
} while(0)

/** @brief Get start of vector as a pointer. */
#define vector_begin(vec) ((vec) ? &(vec)[0] : NULL)

/** @brief Get end of vector as a pointer. */
#define vector_end(vec) ((vec) ? &(vec)[vector_size(vec)] : NULL)

/** @brief Append an element to the end of a vector. */
#ifdef LOGARITHMIC_GROWTH
#define vector_push_back(vec, value) do { \
	size_t __cap = vector_capacity(vec); \
	if(__cap <= vector_size(vec)) { \
		vector_grow((vec), !__cap ? __cap+1 : __cap*2); \
	} \
	vec[vector_size(vec)] = (value); \
	vector_set_size((vec), vector_size(vec)+1); \
} while(0)
#else
#define vector_push_back(vec, value) do { \
	size_t __cap = vector_capacity(vec); \
	if(__cap <= vector_size(vec)) { \
		vector_grow((vec), __cap+1); \
	} \
	vec[vector_size(vec)] = (value); \
	vector_set_size((vec), vector_size(vec)+1); \
} while(0)
#endif

/** @brief Copy vector (from) to vector (to). */
#define vector_copy(from, to) do { \
	size_t __i, __cur_size = vector_size(from); \
	for(__i = 0; __i < __cur_size; __i++) { \
		vector_push_back(to, from[__i]); \
	} \
} while(0)

#ifdef __cplusplus
extern "C" {
#endif

void *vector_pop_back(void *vec);

#ifdef __cplusplus
}
#endif

#endif
