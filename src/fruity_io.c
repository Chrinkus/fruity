/* fruity_io.c
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
#include "fruity_io.h"
#include "fruity.h"
#include <stdio.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Global Variables
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

char fruity_io_delimiter = FRUITY_IO_DELIMITER_DEFAULT;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Row Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
fruity_io_newline(Fruity2DConstData arr, int rows, void* data)
{
	(void)arr;
	(void)rows;
	(void)data;

	const char* newline = fruity_io_delimiter != '\0' ? "\b \b\n" : "\n";

	printf("%s", newline);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Column Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
fruity_io_print_int(Fruity2DConstData arr, int row, int col, void* data)
{
	const int*const*const ai = (const int*const*const)arr;
	int width = data ? *(int*)data : 0;

	printf("%*d%c", width, ai[row][col], fruity_io_delimiter);
}

void
fruity_io_print_char(Fruity2DConstData arr, int row, int col, void* data)
{
	const char*const*const ac = (const char*const*const)arr;
	(void)data;

	printf("%c%c", ac[row][col], fruity_io_delimiter);
}

