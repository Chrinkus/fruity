/* fruity.h
 *
 * MIT License
 * 
 * Copyright (c) 2022 Chris Schick
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

//#include <stddef.h>     /* size_t */

/**
 * Fruity Types
 */
typedef struct fruity_2d Fruity2D;
struct fruity_2d {
        void** data;
        int rows;
        int cols;
};

/*
typedef const void*const*const Fruity2DConst;
typedef void** Fruity2DMutable;

#define fruity_cast_const(pp) (Fruity2DConst)(pp)
#define fruity_cast_mutable(pp) (Fruity2DMutable)(pp)
*/

/**
 * Fruity Function Signatures
 */
/*
typedef void (*FruityRowFuncConst)(Fruity2DConst, int, void*);
typedef void (*FruityColFuncConst)(Fruity2DConst, int, int, void*);

typedef void (*FruityRowFuncMutable)(Fruity2DMutable, int, void*);
typedef void (*FruityColFuncMutable)(Fruity2DMutable, int, int, void*);
*/

/**
 * Standard Library Function Wrappers
 *
 * Removes the need to silently include 'stdlib.h'
 */

/**
 * fruity_malloc
 *
 * Allocate enough space for a 2-dimensional array of the indicated type size.
 * Do not call this function directly, use `fruity_new` instead.
 *
 * @param rows          The number of rows to allocate.
 * @param cols          The number of columns per row.
 * @param type_size     The size of the array type.
 * @param ptr_size      The size of the pointer to the array type.
 *
 * @return              A pointer to a block of memory sufficiently sized to
 *                      contain a 2-dimensional array of the indicated rows,
 *                      columns and type.
 */
void*
fruity_malloc(int rows, int cols, int type_size, int ptr_size);

/**
 * fruity_free
 *
 * De-allocate the memory previously allocated by a call to `fruity_new`.
 *
 * @param ptr   A pointer to an allocated struct fruity_2d.
 */
void
fruity_free(struct fruity_2d* ptr);

/**
 * fruity_new
 *
 * Allocate a new 2D array of the given type. Assign the array to the
 * provided out-pointer.
 *
 * @param t     Type of elements.
 * @param rows  Number of rows.
 * @param cols  Number of columns.
 * @param out   Out pointer to struct fruity_2d.
 */
#define fruity_new(t, num_rows, num_cols, out)                          \
        do {                                                            \
                int r = (num_rows);                                     \
                int c = (num_cols);                                     \
                t** ppt = fruity_malloc(r, c, sizeof(t), sizeof(t*));   \
                if (ppt) {                                              \
                        t* p = (t*)(ppt + r);                           \
                        for (int i = 0; i < r; ++i)                     \
                                ppt[i] = (p + i * c);                   \
                        (out)->data = (void**)ppt;                      \
                        (out)->rows = r;                                \
                        (out)->cols = c;                                \
                }                                                       \
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
/*
void
fruity_foreach(Fruity2DConst a, int rows, int cols,
		FruityRowFuncConst row_func,
		FruityColFuncConst col_func,
		void* userdata);

*/
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
/*
void
fruity_transform(Fruity2DMutable a, int rows, int cols,
		FruityRowFuncMutable row_func,
		FruityColFuncMutable col_func,
		void* userdata);
*/

