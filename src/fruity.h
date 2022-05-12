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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * struct fruity_2d
 *
 * A 2-dimensional array structure. Typedef'd as Fruity2D for ease of use.
 */
typedef struct fruity_2d Fruity2D;
struct fruity_2d {
        void** data;
        int rows;
        int cols;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Function Signatures
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef const void*const*const Fruity2DConstData;
typedef void** Fruity2DMutableData;

typedef void (*FruityRowFunctionConst)(Fruity2DConstData, int, void*);
typedef void (*FruityColFunctionConst)(Fruity2DConstData, int, int, void*);

typedef void (*FruityRowFunctionMutable)(Fruity2DMutableData, int, void*);
typedef void (*FruityColFunctionMutable)(Fruity2DMutableData, int, int, void*);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Functions
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
fruity_new(struct fruity_2d* pfs, int rows, int cols, int size);

/**
 * fruity_free
 *
 * De-allocate the memory previously allocated by a call to `fruity_new`.
 *
 * @param pfs   A pointer to a fruity_2d struct.
 */
void
fruity_free(struct fruity_2d* pfs);

/**
 * fruity_data
 *
 * Get a pointer to the data member of the fruity_2d struct. This is a
 * convenience function for accessing the inner data for double bracket
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
fruity_data(struct fruity_2d* pfs)
{
        return (void*)pfs->data;
}

/**
 * fruity_foreach
 *
 * Perform operations on a read-only 2D array.
 *
 * @param pfs           A read-only pointer to the fruity_2d struct.
 * @param row_func      A function to execute at the end of each row.
 * @param col_func      A function to execute for each element of the array.
 * @param user_data     Optional data to be passed to row and column functions.
 */
void
fruity_foreach(const struct fruity_2d* pfs, 
                FruityRowFunctionConst row_func,
                FruityColFunctionConst col_func,
                void* userdata);
/**
 * fruity_transform
 *
 * Perform operations on a mutable fruity_2d struct, possibly altering its
 * data.
 *
 * @param pfs           A pointer to the fruity_2d struct to operate on.
 * @param row_func      A function to execute at the end of each row.
 * @param col_func      A function to execute for each element of the array.
 * @param user_data     Optional data to be passed to row and column functions.
 */
void
fruity_transform(struct fruity_2d* pfs,
                 FruityRowFunctionMutable row_func,
                 FruityColFunctionMutable col_func,
                 void* userdata);

/**
 * fruity_init
 *
 * Initialize all of the elements of a fruity_2d struct to a provided value.
 *
 * @param pfs   A pointer to the fruity_2d struct to initialize.
 * @param value A read-only pointer to a value to use as initial data.
 * @param size  The size in bytes of the value.
 */
void
fruity_init(struct fruity_2d* pfs, const void* value, int size);

