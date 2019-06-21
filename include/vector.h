#ifndef PRS_VECTOR_H
#define PRS_VECTOR_H

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief For internal use only.
 */
#define _vector_set_capacity(vec, size) \
do { \
	if(vec) { \
		((size_t*)(vec))[-1] = (size); \
	} \
} while(0)

/**
 * @brief For internal use only.
 */
#define _vector_set_size(vec, size) \
do { \
	if(vec) { \
		((size_t*)(vec))[-2] = (size); \
	} \
} while(0)

/**
 * @brief Gets the current capacity of the vector.
 */
#define vector_capacity(vec) \
	((vec) ? ((size_t*)(vec))[-1] : (size_t)0)

/**
 * @brief Gets the current size of the vector.
 */
#define vector_size(vec) \
	((vec) ? ((size_t*)(vec))[-2] : (size_t)0)

/**
 * @brief Returns non-zero if the vector is empty.
 */
#define vector_empty(vec) \
	(vector_size(vec) == 0)

/**
 * @brief For internal use only, grows vector.
 */
#define _vector_grow(vec, count) \
do { \
	if(!(vec)) { \
		size_t *__p; \
		__p = malloc((count)*sizeof(*(vec))+(sizeof(size_t)*2)); \
		assert(__p); \
		(vec) = (void*)(&__p[2]); \
		_vector_set_capacity((vec), (count)); \
		_vector_set_size((vec), 0); \
	} else { \
		size_t *__p1,*__p2; \
		__p1 = &((size_t*)(vec))[-2]; \
		__p2 = \
		realloc(__p1, ((count)*sizeof(*(vec))+(sizeof(size_t)*2))); \
		assert(__p2); \
		(vec) = (void*)(&__p2[2]); \
		_vector_set_capacity((vec), (count)); \
	} \
} while(0)

/**
 * @brief Removes the last element from the vector.
 */
#define vector_pop_back(vec) \
do { \
	_vector_set_size((vec), vector_size(vec)-1); \
} while(0)

/**
 * @brief Removes the element at i index from the vector.
 */
#define vector_erase(vec, i) \
do { \
	if(vec) { \
		const size_t __sz = vector_size(vec); \
		if((i) < __sz) { \
			size_t __x;
			_vector_set_size((vec), __sz-1); \
			for(__x=(i); __x < (__sz-1); ++__x) { \
				(vec)[__x] = (vec)[__x+1]; \
			} \
		} \
	} \
} while(0)

/**
 * @brief Frees all memory associated with the vector.
 */
#define vector_free(vec) \
do { \
	if(vec) { \
		size_t *p1 = &((size_t*)(vec))[-2]; \
		free(p1); \
	} \
} while(0)

/**
 * @brief Returns an iterator to first element of the vector.
 */
#define vector_begin(vec) (vec)

/**
 * @brief Returns an iterator one past the last element of the vector.
 */
#define vector_end(vec) ((vec) ? &((vec)[vector_size(vec)]) : NULL)

/**
 * @brief Adds an element to the end of the vector.
 */
#ifdef LOGARITHMIC_GROWTH
#define vector_push_back(vec, value) \
do { \
	size_t __cap = vector_capacity(vec); \
	if(__cap <= vector_size(vec)) { \
		_vector_grow((vec), !__cap ? __cap+1 : __cap*2); \
	} \
	vec[vector_size(vec)] = (value); \
	_vector_set_size((vec), vector_size(vec)+1); \
} while(0)
#else
#define vector_push_back(vec, value) \
do { \
	size_t __cap = vector_capacity(vec); \
	if(__cap <= vector_size(vec)) { \
		_vector_grow((vec), __cap+1); \
	} \
	vec[vector_size(vec)] = (value);
	_vector_set_size((vec), vector_size(vec)+1); \
} while(0)
#endif

#endif
