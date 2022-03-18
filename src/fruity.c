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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Standard Library Wrappers
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void*
fruity_malloc(size_t rows, size_t cols, size_t type_size, size_t ptr_size)
{
	return malloc(ptr_size * rows + type_size * rows * cols);
}

void
fruity_free(void* arr)
{
	free(arr);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
fruity_foreach(Fruity2DConst a, int rows, int cols,
		FruityRowFuncConst row_func,
		FruityColFuncConst col_func,
		void* userdata)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			if (col_func)
				col_func(a, i, j, userdata);
		if (row_func)
			row_func(a, i, userdata);
	}
}

void
fruity_transform(Fruity2DMutable a, int rows, int cols,
		FruityRowFuncMutable row_func,
		FruityColFuncMutable col_func,
		void* userdata)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			if (col_func)
				col_func(a, i, j, userdata);
		if (row_func)
			row_func(a, i, userdata);
	}
}

