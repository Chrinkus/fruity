#pragma once

#include <stddef.h>

/**
 * Fruity Const-ness
 */
typedef const void** FruityConst2D;
typedef void** FruityMutable2D;

#define fruity_cast(pp) (FruityConst2D)(pp)
#define fruity_cast_mutable(pp) (FruityMutable2D)(pp)

/**
 * Fruity function signatures
 */
typedef void (*FruityRowFuncConst)(FruityConst2D, int, void*);
typedef void (*FruityColFuncConst)(FruityConst2D, int, int, void*);

typedef void (*FruityRowFuncMutable)(FruityMutable2D, int, void*);
typedef void (*FruityColFuncMutable)(FruityMutable2D, int, int, void*);

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

/**
 * fruity_foreach
 *
 * Perform operations on a read-only 2D array.
 *
 * @param a		The 2D array to read from.
 * @param rows		The number of rows in the 2D array.
 * @param cols		The number of columns in the 2D array.
 * @param row_func	A function to execute at the end of each row.
 * @param col_func	A function to execute for each element of the array.
 * @param user_data	Optional data to be passed to row and column functions.
 */
void
fruity_foreach(FruityConst2D a, int rows, int cols,
		FruityRowFuncConst row_func,
		FruityColFuncConst col_func,
		void* userdata);

/**
 * fruity_transform
 *
 * Perform operations on a mutable 2D array, possibly altering its data.
 *
 * @param a		The 2D array to operate on.
 * @param rows		The number of rows in the 2D array.
 * @param cols		The number of columns in the 2D array.
 * @param row_func	A function to execute at the end of each row.
 * @param col_func	A function to execute for each element of the array.
 * @param user_data	Optional data to be passed to row and column functions.
 */
void
fruity_transform(FruityMutable2D a, int rows, int cols,
		FruityRowFuncMutable row_func,
		FruityColFuncMutable col_func,
		void* userdata);

