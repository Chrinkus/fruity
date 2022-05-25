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
 * Convenience Types and Defs
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

typedef char FruityByte;
typedef FruityByte** Fruity2DByteData;

#define FRUITY_2D_BYTES(pp) (Fruity2DByteData)(pp)
#define FRUITY_2D_CONST(pp) (Fruity2DConstData)(pp)
#define FRUITY_2D_MUTABLE(pp) (Fruity2DMutableData)(pp)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity inline getter symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void* 
fruity_data(struct fruity_2d* pfs);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
fruity_new(struct fruity_2d* pfs, int rows, int cols, int size)
{
        char** pp = malloc(rows * sizeof(char*) + rows * cols * size);

        if (pp) {
                char* arr = (char*)(pp + rows);
                for (int i = 0; i < rows; ++i)
                        pp[i] = (arr + i * cols * size);
                pfs->data = FRUITY_2D_MUTABLE(pp);
                pfs->rows = rows;
                pfs->cols = cols;
                pfs->size = size;
        }

        return (void*)pp;
}

void
fruity_free(struct fruity_2d* pfs)
{
        free(pfs->data);
        memset(pfs, 0, sizeof(struct fruity_2d));
}

void
fruity_foreach(const struct fruity_2d* pfs,
                FruityRowFunctionConst row_func,
                FruityColFunctionConst col_func,
                void* userdata)
{
        Fruity2DConstData p = FRUITY_2D_CONST(pfs->data);

        for (int i = 0; i < pfs->rows; ++i) {
                for (int j = 0; j < pfs->cols; ++j)
                        if (col_func)
                                col_func(p, i, j, userdata);
                if (row_func)
                        row_func(p, i, userdata);
        }
}

void
fruity_transform(struct fruity_2d* pfs,
                 FruityRowFunctionMutable row_func,
                 FruityColFunctionMutable col_func,
                 void* userdata)
{
        Fruity2DMutableData p = FRUITY_2D_MUTABLE(pfs->data);

        for (int i = 0; i < pfs->rows; ++i) {
                for (int j = 0; j < pfs->cols; ++j)
                        if (col_func)
                                col_func(p, i, j, userdata);
                if (row_func)
                        row_func(p, i, userdata);
        }
}

void
fruity_init(struct fruity_2d* pfs, const void* value)
{
        Fruity2DByteData p = FRUITY_2D_BYTES(pfs->data);

        const int sz = pfs->size;
        for (int i = 0; i < pfs->rows; ++i)
                for (int j = 0; j < pfs->cols; ++j)
                        memcpy(&p[i][j * sz], value, sz);
}

int
fruity_adjacent_4(struct fruity_2d* pfs, const int r, const int c,
                struct fruity_2d_cell adj[4])
{
        int count = 0;
        const int sz = pfs->size;

        Fruity2DByteData p = FRUITY_2D_BYTES(pfs->data);

        if (r - 1 >= 0)         // UP
                adj[count++] = (struct fruity_2d_cell){
                        .ptr = &p[r - 1][c * sz],
                        .row = r - 1,
                        .col = c,
                };
        if (c + 1 < pfs->cols)  // RIGHT
                adj[count++] = (struct fruity_2d_cell){
                        .ptr = &p[r][(c + 1) * sz],
                        .row = r,
                        .col = c + 1,
                };
        if (r + 1 < pfs->rows)  // DOWN
                adj[count++] = (struct fruity_2d_cell){
                        .ptr = &p[r + 1][c * sz],
                        .row = r + 1,
                        .col = c,
                };
        if (c - 1 >= 0)         // LEFT
                adj[count++] = (struct fruity_2d_cell){
                        .ptr = &p[r][(c - 1) * sz],
                        .row = r,
                        .col = c - 1,
                };

        return count;
}

