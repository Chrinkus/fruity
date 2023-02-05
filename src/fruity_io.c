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

#include "cgs.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Global Variables
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

char fruity_io_delimiter = FRUITY_IO_DELIMITER_DEFAULT;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Row Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
fruity_io_newline(void* row_data, void* col_data)
{
	(void)row_data;
	(void)col_data;

	const char* newline = fruity_io_delimiter != '\0' ? "\b \b\n" : "\n";

	printf("%s", newline);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Column Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

void
fruity_io_print_int(struct f2d_cell cell, void* data)
{
        const int* p = cell.data;
	int width = data ? *(int*)data : 0;

	printf("%*d%c", width, *p, fruity_io_delimiter);
}

void
fruity_io_print_char(struct f2d_cell cell, void* data)
{
        const char* p = cell.data;
	(void)data;

	printf("%c%c", *p, fruity_io_delimiter);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Reading Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static int
test_all_lengths_equal(const struct cgs_vector* lines)
{
        if (cgs_vector_length(lines) < 1)
                return 1;
        const struct cgs_string* s = cgs_vector_get(lines, 0);

        size_t len = cgs_string_length(s);
        for (size_t i = 1; i < cgs_vector_length(lines); ++i)
                if (len != cgs_string_length(cgs_vector_get(lines, i)))
                        return 0;
        return 1;
}

void*
fruity_io_read_chars(struct fruity_2d* f2d, FILE* file)
{
        struct cgs_vector lines = cgs_vector_new(sizeof(struct cgs_string));
        if (!cgs_io_readlines(file, &lines) || !test_all_lengths_equal(&lines))
                return NULL;

        const int rows = (int)cgs_vector_length(&lines);
        const int cols = (int)cgs_string_length(cgs_vector_get(&lines, 0));

        if (!fruity_build(f2d, rows, cols, NULL, sizeof(char)))
                return NULL;

        char** aac = fruity_data_mut(f2d);
        for (int i = 0; i < rows; ++i) {
                const char* s = cgs_string_data(cgs_vector_get(&lines, i));
                for (int j = 0; j < cols; ++j)
                        aac[i][j] = s[j];
        }

        cgs_vector_free_all_with(&lines, cgs_string_free);
        return f2d;
}
