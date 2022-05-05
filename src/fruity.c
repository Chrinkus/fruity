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
 * Standard Library Wrappers
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
fruity_malloc(int rows, int cols, int type_size, int ptr_size)
{
        return malloc(ptr_size * rows + type_size * rows * cols);
}

void
fruity_free(struct fruity_2d* pfs)
{
        free(pfs->data);
        pfs->data = NULL;
        pfs->rows = 0;
        pfs->cols = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity inline getter symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void* 
fruity_data(struct fruity_2d* pfs);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
fruity_foreach(const struct fruity_2d* pfs,
                FruityRowFuncConst row_func,
                FruityColFuncConst col_func,
                void* userdata)
{
        Fruity2DConst p = FRUITY_CAST_CONST(pfs->data);

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
                 FruityRowFuncMutable row_func,
                 FruityColFuncMutable col_func,
                 void* userdata)
{
        Fruity2DMutable p = FRUITY_CAST_MUTABLE(pfs->data);

        for (int i = 0; i < pfs->rows; ++i) {
                for (int j = 0; j < pfs->cols; ++j)
                        if (col_func)
                                col_func(p, i, j, userdata);
                if (row_func)
                        row_func(p, i, userdata);
        }
}

void
fruity_initialize(struct fruity_2d* pfs, const void* value, int size)
{
        Fruity2DBytes p = FRUITY_CAST_BYTES(pfs->data);

        for (int i = 0; i < pfs->rows; ++i)
                for (int j = 0; j < pfs->cols; ++j)
                        memcpy(&p[i][j * size], value, size);
}

