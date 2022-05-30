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
 *
 * All structs are typedef'd for ease of use.
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
typedef struct fruity_2d Fruity2D;
struct fruity_2d {
        char** data;
        int rows;
        int cols;
        int size;
};

/**
 * struct fruity_2d_cell
 *
 * An cell within a 2D structure. Contains both a pointer to the element
 * and the row and column of the element.
 *
 * @member ptr  A pointer to the element.
 * @member row  The row of the element.
 * @member col  The column of the element.
 */
typedef struct fruity_2d_cell Fruity2DCell;
struct fruity_2d_cell {
        void* ptr;
        int row;
        int col;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Function Signatures
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef void (*FruityRowFunction)(void* row_data, void* col_data);

typedef void (*FruityColFunction)(Fruity2DCell cell, void* col_data);

typedef void (*FruityColFunctionConst)(const void* element, void* col_data);
typedef void (*FruityColFunctionMutable)(void* element, void* col_data);

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
 * fruity_get_mutable
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
fruity_get_mutable(struct fruity_2d* pfs, int row, int col)
{
        return &((char**)pfs->data)[row][col * pfs->size];
}

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
                FruityRowFunction row_func,
                void* row_data,
                //FruityColFunctionConst col_func,
                FruityColFunction col_func,
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
                 FruityRowFunction row_func,
                 void* row_data,
                 //FruityColFunctionMutable col_func,
                 FruityColFunction col_func,
                 void* col_data);

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
fruity_adjacent_4(struct fruity_2d* pfs, const int r, const int c,
                struct fruity_2d_cell adj[4]);

