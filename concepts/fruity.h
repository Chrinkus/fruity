#pragma once

/**
 * fruity_new
 *
 * Allocate a new 2D array of the given type. Assign the array to the
 * provided out-pointer.
 *
 * @param t	Type of elements.
 * @param rows	Number of rows.
 * @param cols	Number of columns.
 * @param pp	Out pointer of type t**.
 */
#define fruity_new(t, rows, cols, pp)					\
	do {								\
		size_t r = (rows);					\
		size_t c = (cols);					\
		pp = malloc(sizeof(t*) * r + sizeof(t) * r * c);	\
		if (pp) {						\
			t* p = (t*)(pp + r);				\
			for (size_t i = 0; i < r; ++i)			\
				pp[i] = (p + i * c);			\
		}							\
	} while (0)

#define fruity_cast(pp) (const void**)(pp)
#define fruity_cast_mutable(pp) (void**)(pp)

