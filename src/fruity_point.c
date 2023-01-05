/* fruity_point.c
 *
 * MIT License
 * 
 * Copyright (c) 2023 Chris Schick
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
#include "fruity_point.h"

enum {
        F2D_POINT_HASH_MAGIC = 0x1A51,
};

int
f2d_point_cmp(const void* a, const void* b)
{
        const struct f2d_point* p1 = a;
        const struct f2d_point* p2 = b;
        int x1 = p1->x;
        int y1 = p1->y;
        int x2 = p2->x;
        int y2 = p2->y;

        if (x1 < x2)
                return -1;
        if (x1 > x2)
                return 1;
        // x's equal, compare y's
        if (y1 < y2)
                return -1;
        if (y1 > y2)
                return 1;
        // points are equal
        return 0;
}

size_t
f2d_point_hash(struct f2d_point pt, size_t buckets)
{
        size_t xx = (size_t)pt.x;
        size_t yy = (size_t)pt.y;
        return (xx + yy * F2D_POINT_HASH_MAGIC) % buckets;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int
f2d_point_row(const struct f2d_point* pt);

int
f2d_point_col(const struct f2d_point* pt);

void
f2d_point_move(struct f2d_point* pt, int dx, int dy);
