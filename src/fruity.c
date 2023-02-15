/* fruity.c
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
#include "fruity.h"
#include <stdlib.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void*
fruity_new(struct fruity_2d* pfs, int rows, int cols, size_t size)
{
        char** pp = malloc(rows * sizeof(char*) + rows * cols * size);

        if (pp) {
                char* arr = (char*)(pp + rows);
                for (int i = 0; i < rows; ++i)
                        pp[i] = (arr + i * cols * size);
                pfs->data = pp;
                pfs->rows = rows;
                pfs->cols = cols;
                pfs->size = size;
        }

        return (void*)pp;
}

void*
fruity_build(struct fruity_2d* pfs, int rows, int cols, const void* init,
                size_t size)
{
        if (rows <= 0 || cols <= 0)
                return NULL;

        char** pp = malloc(rows * sizeof(char*) + rows * cols * size);
        if (!pp)
                return NULL;

        // Set pointer array to start of each row
        char* arr = (char*)(pp + rows);
        for (int i = 0; i < rows; ++i)
                pp[i] = (arr + i * cols * size);

        // Set initial value of elements if given
        if (init) {
                for (int i = 0; i < rows; ++i)
                        for (int j = 0; j < cols; ++j)
                                memcpy(&pp[i][j * size], init, size);
        } else {
                memset(&pp[0][0], 0, rows * cols * size);
        }

        // Set members
        pfs->data = pp;
        pfs->rows = rows;
        pfs->cols = cols;
        pfs->size = size;
        return pfs;
}

void*
fruity_copy(const struct fruity_2d* src, struct fruity_2d* dst)
{
        if (!fruity_new(dst, src->rows, src->cols, src->size))
                return NULL;

        const void* psrc = fruity_get(src, 0, 0);
        void* pdst = fruity_get_mut(dst, 0, 0);

        memcpy(pdst, psrc, src->size * src->rows * src->cols);
        return dst;
}

void
fruity_move(struct fruity_2d* src, struct fruity_2d* dst)
{
        fruity_free(dst);
        dst->data = src->data;
        dst->rows = src->rows;
        dst->cols = src->cols;
        dst->size = src->size;

        memset(src, 0, sizeof(*src));
}

void*
fruity_grow(struct fruity_2d* pfs, int nrows, int ncols,
                unsigned srows, unsigned scols, void* init)
{
        if (nrows == 0 && ncols == 0)
                return pfs;     // nothing to do
        if (nrows < 0 || ncols < 0)
                return NULL;    // error: not a shrinking function
        if (srows > (unsigned)nrows || scols > (unsigned)ncols)
                return NULL;    // error: shift larger than added dimension

        struct fruity_2d tmp = { 0 };
        if (!fruity_build(&tmp, pfs->rows + nrows, pfs->cols + ncols, init,
                                pfs->size))
                return NULL;

        for (int i = 0; i < fruity_rows(pfs); ++i)
                for (int j = 0; j < fruity_cols(pfs); ++j) {
                        void* dst = fruity_get_mut(&tmp, i + srows, j + scols);
                        const void* src = fruity_get(pfs, i, j);
                        memcpy(dst, src, pfs->size);
                }

        fruity_move(&tmp, pfs);
        return pfs;
}

void
fruity_free(void* p)
{
        struct fruity_2d* pfs = p;

        free(pfs->data);
        memset(pfs, 0, sizeof(struct fruity_2d));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity inline getter symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int
fruity_rows(const struct fruity_2d* pfs);

int
fruity_cols(const struct fruity_2d* pfs);

const void* 
fruity_data(const struct fruity_2d* pfs);

void* 
fruity_data_mut(struct fruity_2d* pfs);

const void*
fruity_get(const struct fruity_2d* pfs, int row, int col);

void*
fruity_get_mut(struct fruity_2d* pfs, int row, int col);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
fruity_init(struct fruity_2d* pfs, const void* value)
{
        char** p = pfs->data;
        const size_t sz = pfs->size;

        for (int i = 0; i < pfs->rows; ++i)
                for (int j = 0; j < pfs->cols; ++j)
                        memcpy(&p[i][j * sz], value, sz);
}

void
fruity_foreach(const struct fruity_2d* pfs,
                F2DRowFunc row_func,
                void* row_data,
                F2DColFunc col_func,
                void* col_data)
{
        char** p = pfs->data;

        for (int i = 0; i < pfs->rows; ++i) {
                for (int j = 0; j < pfs->cols; ++j)
                        if (col_func)
                                col_func((struct f2d_cell){
                                        .data = &p[i][j*pfs->size],
                                        .pt = { .y = i, .x = j },
                                }, col_data);
                if (row_func)
                        row_func(row_data, col_data);
        }
}

void
fruity_transform(struct fruity_2d* pfs,
                 F2DRowFunc row_func,
                 void* row_data,
                 F2DColFuncMut col_func,
                 void* col_data)
{
        char** p = pfs->data;

        for (int i = 0; i < pfs->rows; ++i) {
                for (int j = 0; j < pfs->cols; ++j)
                        if (col_func)
                                col_func((struct f2d_cell_mut){
                                        .data = &p[i][j*pfs->size],
                                        .pt = { .y = i, .x = j },
                                }, col_data);
                if (row_func)
                        row_func(row_data, col_data);
        }
}

int
fruity_count_if(const struct fruity_2d* pfs,
                F2DPredicate pred,
                void* data)
{
        int sum = 0;
        char** p = pfs->data;
        for (int i = 0; i < pfs->rows; ++i)
                for (int j = 0; j < pfs->cols; ++j) {
                        struct f2d_cell cell = {
                                .data = &p[i][j * pfs->size],
                                .pt = { .y = i, .x = j },
                        };
                        if (pred(cell, data))
                                ++sum;
                }

        return sum;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Fruity Pathfinding
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int
fruity_adjacent_4(const struct fruity_2d* pfs, int r, int c,
                struct f2d_cell adj[4])
{
        int count = 0;
        const char*const* p = fruity_data(pfs);
        const size_t sz = pfs->size;

        if (r != 0)                     // UP
                adj[count++] = (struct f2d_cell){
                        .data = &p[r - 1][c * sz],
                        .pt = { .y = r - 1, .x = c },
                };
        if (c + 1 < fruity_cols(pfs))   // RIGHT
                adj[count++] = (struct f2d_cell){
                        .data = &p[r][(c + 1) * sz],
                        .pt = { .y = r, .x = c + 1 },
                };
        if (r + 1 < fruity_rows(pfs))   // DOWN
                adj[count++] = (struct f2d_cell){
                        .data = &p[r + 1][c * sz],
                        .pt = { .y = r + 1, .x = c },
                };
        if (c != 0)                     // LEFT
                adj[count++] = (struct f2d_cell){
                        .data = &p[r][(c - 1) * sz],
                        .pt = { .y = r, .x = c - 1 },
                };

        return count;
}
