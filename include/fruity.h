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

#include <stddef.h>
#include "fruity_point.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * struct fruity_2d
 *
 * A 2-dimensional array structure.
 *
 * @member data A pointer to the allocated structure.
 * @member rows The number of rows in the 2D structure.
 * @member cols The number of columns in the 2D structure.
 * @member size The size of the elements of the 2D structure in bytes.
 */
struct fruity_2d {
        char** data;
        int rows;
        int cols;
        size_t size;
};

/**
 * struct f2d_cell
 *
 * A cell within a 2D structure. Contains both a read-only pointer to the
 * element and a point containing the row and column location of the element.
 *
 * @member data A read-only pointer to the element.
 * @member pt   The location of the element.
 */
struct f2d_cell {
        const void* data;
        struct f2d_point pt;
};

/**
 * struct f2d_cell_mut
 *
 * A cell within a 2D structure. Contains both a mutable pointer to the 
 * element and a point containing the row and column location of the element.
 *
 * @member data A mutable pointer to the element.
 * @member pt   The location of the element.
 */
struct f2d_cell_mut {
        void* data;
        struct f2d_point pt;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Function Signatures
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * F2DRowFunc
 *
 * The signature of a function to be passed to 'foreach' and 'transform' that
 * will be called at the end of processing every row.
 *
 * - foreach
 * - transform
 *
 * @param row_data      A pointer to the user data passed for row processing.
 * @param col_data      A pointer to the user data passed for column processing.
 */
typedef void (*F2DRowFunc)(void* row_data, void* col_data);

/**
 * F2DColFunc
 *
 * The signature of a function to be passed to fruity functions that only
 * require read-access to the elements of the 2D array.
 *
 * - foreach
 *
 * @param cell  An f2d_cell containing a pointer to the current element as
 *              well as the element's location.
 * @param data  A pointer to the user data passed for column processing.
 */
typedef void (*F2DColFunc)(struct f2d_cell cell, void* data);

/**
 * F2DColFuncMut
 *
 * The signature of a function to be passed to fruity functions that intend
 * to mutate the elements of the 2D array.
 *
 * - transform
 *
 * @param cell  An f2d_cell_mut containing a pointer to the current element
 *              as well as the element's location.
 * @param data  A pointer to the user data passed for column processing.
 */
typedef void (*F2DColFuncMut)(struct f2d_cell_mut cell, void* data);

/**
 * F2DPredicate
 *
 * The signature of a function to be passed to fruity functions that evaluate
 * a boolean condition on the elements of a 2D array.
 *
 * - count_if
 *
 * @param cell  An f2d_cell containing a pointer to the current element as well
 *              as the element's location.
 * @param data  A pointer to optional userdata.
 *
 * @return      An integer boolean value.
 */
typedef int (*F2DPredicate)(struct f2d_cell cell, void* data);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_new
 *
 * Allocate a new 2D array of the given type. Assign the array to the data
 * member of the provided fruity_2d struct. Set rows and columns.
 *
 * @param pfs   Out pointer to fruity_2d struct.
 * @param rows  Number of rows.
 * @param cols  Number of columns.
 * @param size  Size of the elements.
 *
 * @return      A pointer to the allocated data on success, NULL on failure.
 */
void*
fruity_new(struct fruity_2d* pfs, int rows, int cols, size_t size);

/**
 * fruity_build
 *
 * Allocate a 2D array of the given dimensions for elements of the given size.
 * If an initial value is provided, set all elements to equal it.
 *
 * @param pfs   The fruity_2d struct.
 * @param rows  The number of rows.
 * @param cols  The number of columns.
 * @param init  A pointer to the desired initial data. May be NULL.
 * @param size  The size of the elements.
 *
 * @return      A pointer to the fruity_2d struct on success, NULL on
 *              failure.
 */
void*
fruity_build(struct fruity_2d* pfs, int rows, int cols, const void* init,
                size_t size);

/**
 * fruity_copy
 *
 * Allocate a new 2D array with the dimensions of another. Fill the new
 * 2D array with a copy of the data in the other.
 *
 * @param src   The source 2D array. (Read-only)
 * @param dst   The destination 2D array object.
 *
 * @return      A pointer to 'dst' on success, NULL on failure.
 */
void*
fruity_copy(const struct fruity_2d* src, struct fruity_2d* dst);

/**
 * fruity_move
 *
 * Moves the contents of 'src' to 'dst', setting members of 'src' to zero.
 * Performs a free on 'dst' before moving.
 *
 * @param src   The 2D array to move.
 * @param dst   The 2D array to move 'src' to.
 */
void
fruity_move(struct fruity_2d* src, struct fruity_2d* dst);

/**
 * fruity_grow
 *
 * Extend the size of the 2D array by a given number of rows and/or columns.
 * Optionally shift the original contents by a given number of rows and/or
 * columns as long as the shifts are not greater than the extended dimensions.
 *
 * @param pfs   The 2D array to grow.
 * @param nrows The number of rows to add.
 * @param ncols The number of columns to add.
 * @param srows The number of rows to shift.
 * @param scols The number of columns to shift.
 * @param init  Optional. A pointer to a desired initial value for the newly
 *              added elements or NULL for zero-initialization.
 *
 * @return      A pointer to the 2D array struct on success or NULL on error.
 */
void*
fruity_grow(struct fruity_2d* pfs, int nrows, int ncols,
                unsigned srows, unsigned scols, void* init);

/**
 * fruity_free
 *
 * De-allocate the memory previously allocated by a call to `fruity_new`.
 *
 * @param p     A pointer to a fruity_2d struct. Passed as 'void*' to mirror
 *              standard library 'free'.
 */
void
fruity_free(void* p);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_rows
 *
 * Row count getter.
 *
 * @param pfs   A read-only pointer to a fruity struct.
 *
 * @return      The number of rows in the struct.
 */
inline int
fruity_rows(const struct fruity_2d* pfs)
{
        return pfs->rows;
}

/**
 * fruity_cols
 *
 * Column count getter.
 *
 * @param pfs   A read-only pointer to a fruity struct.
 *
 * @return      The number of columns in the struct.
 */
inline int
fruity_cols(const struct fruity_2d* pfs)
{
        return pfs->cols;
}

/**
 * fruity_data
 *
 * Get a read-only pointer to the data member of the fruity_2d struct. This
 * is a convenience function for accessing the inner data for double bracket
 * referencing.
 *
 * Warning: The 'data' member is cast to a void* on return to alleviate
 * any compiler warnings. The caller must be putting this into the
 * appropriate type. See tests_fruity.c for examples.
 *
 * @param pfs   A pointer to a fruity_2d struct.
 *
 * @return      A read-only pointer to the data member of 'pfs'.
 */
inline const void*
fruity_data(const struct fruity_2d* pfs)
{
        return (void*)pfs->data;
}

/**
 * fruity_data_mut
 *
 * Get a writable pointer to the data member of the fruity_2d struct. This
 * is a convenience function for accessing the inner data for double bracket
 * referencing.
 *
 * Warning: The 'data' member is cast to a void* on return to alleviate
 * any compiler warnings. The caller must be putting this into the
 * appropriate type. See tests_fruity.c for examples.
 *
 * @param pfs   A pointer to a fruity_2d struct.
 *
 * @return      A pointer to the data member of 'pfs'.
 */
inline void* 
fruity_data_mut(struct fruity_2d* pfs)
{
        return (void*)pfs->data;
}

/**
 * fruity_get
 *
 * Get a read-only pointer to a cell in a fruity_2d struct.
 *
 * @param pfs   A read-only pointer to the fruity_2d struct.
 * @param row   The row location of the cell.
 * @param col   The column location of the cell.
 *
 * @return      A read-only pointer to the cell.
 */
inline const void*
fruity_get(const struct fruity_2d* pfs, int row, int col)
{
        return &((char**)pfs->data)[row][col * pfs->size];
}

/**
 * fruity_get_mut
 *
 * Get a pointer to a cell in a fruity_2d struct.
 *
 * @param pfs   A pointer to the fruity_2d struct.
 * @param row   The row location of the cell.
 * @param col   The column location of the cell.
 *
 * @return      A mutable pointer to the cell.
 */
inline void*
fruity_get_mut(struct fruity_2d* pfs, int row, int col)
{
        return &((char**)pfs->data)[row][col * pfs->size];
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_init
 *
 * Initialize all of the elements of a fruity_2d struct to a provided value.
 *
 * @param pfs   A pointer to the fruity_2d struct to initialize.
 * @param value A read-only pointer to a value to use as initial data.
 */
void
fruity_init(struct fruity_2d* pfs, const void* value);

/**
 * fruity_foreach
 *
 * Perform operations on a read-only 2D array.
 *
 * @param pfs           A read-only pointer to the fruity_2d struct.
 * @param row_func      A function to execute at the end of each row.
 * @param row_data      Optional data to be passed to row function.
 * @param col_func      A function to execute for each element of the array.
 * @param col_data      Optional data to be passed to row and column functions.
 */
void
fruity_foreach(const struct fruity_2d* pfs, 
                F2DRowFunc row_func,
                void* row_data,
                F2DColFunc col_func,
                void* col_data);

/**
 * fruity_transform
 *
 * Perform operations on a mutable fruity_2d struct, possibly altering its
 * data.
 *
 * @param pfs           A pointer to the fruity_2d struct to operate on.
 * @param row_func      A function to execute at the end of each row.
 * @param row_data      Optional data to be passed to row function.
 * @param col_func      A function to execute for each element of the array.
 * @param col_data      Optional data to be passed to row and column functions.
 */
void
fruity_transform(struct fruity_2d* pfs,
                 F2DRowFunc row_func,
                 void* row_data,
                 F2DColFuncMut col_func,
                 void* col_data);

/**
 * fruity_count_if
 *
 * Count the number of elements in a fruity_2d struct that satisfy a given
 * predicate.
 *
 * @param pfs   A read-only pointer to the fruity_2d struct to evaluate.
 * @param pred  A function that takes a pointer to an element of the 2D array
 *              and returns an integer boolean.
 * @param data  Optional userdata to be passed to the predicate.
 *
 * @return      An integer count of the number of elements that pass the
 *              predicate.
 */
int
fruity_count_if(const struct fruity_2d* pfs,
                F2DPredicate pred,
                void* data);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Pathfinding
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_adjacent_4
 *
 * Get pointers to the 4 adjacent elements up, right, down, and left of the
 * provided row and column coordinates. If the coordinates are on the edge
 * there will be less than 4 values in the out array.
 *
 * The caller may need to cast their out array to void** to silence warnings.
 *
 * Returns the number of adjacents in the array.
 *
 * @param pfs   A pointer to the fruity_2d struct.
 * @param r     The row of the source coordinate.
 * @param c     The column of the source coordinate.
 * @param adj   An array of fruity 2D element structs.
 *
 * @return      The number of adjacent elements found.
 */
int
fruity_adjacent_4(const struct fruity_2d* pfs, int r, int c,
                struct f2d_cell adj[4]);

int
fruity_adjacent_8(const struct fruity_2d* pfs, int r, int c,
                struct f2d_cell* adj);
