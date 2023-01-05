/* fruity_point.h
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
#pragma once

#include <stddef.h>

/**
 * struct f2d_point
 *
 * A set of coordinates to a location within a 2D array.
 *
 * @member x    The column on the 'x' axis.
 * @member y    The row on the 'y' axis.
 */
struct f2d_point {
        int x;
        int y;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * f2d_point_row
 *
 * Row getter.
 *
 * @param pt    The point.
 *
 * @return      The row position.
 */
inline int
f2d_point_row(const struct f2d_point* pt)
{
        return pt->y;
}

/**
 * f2d_point_col
 *
 * Column getter.
 *
 * @param pt    The point.
 *
 * @return      The column position.
 */
inline int
f2d_point_col(const struct f2d_point* pt)
{
        return pt->x;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Point Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

/**
 * f2d_point_move
 *
 * Move a given point according using deltas for each dimension.
 *
 * @param pt    The point to move.
 * @param dx    The change to x.
 * @param dy    The change to y.
 */
inline void
f2d_point_move(struct f2d_point* pt, int dx, int dy)
{
        pt->x += dx;
        pt->y += dy;
}

/**
 * f2d_point_cmp
 *
 * Perform a three-way comparison between two points. Top left < bottom right.
 * Points passed as `void*`s to match standard library comparison operations.
 *
 * @param a     A read-only pointer to the target point.
 * @param b     A read-only pointer to the compare point.
 *
 * @return      An integer indicating the result of the comparison: 0 if equal,
 *              < 0 for lesser, > 0 for greater.
 */
int
f2d_point_cmp(const void* a, const void* b);

/**
 * f2d_point_hash
 *
 * Calculate a hash value for a point that fits into a given sized set of
 * buckets.
 *
 * @param pt            A point.
 * @param buckets       The number of buckets in the table.
 *
 * @return              An unsigned value in the range of [0:buckets).
 */
size_t
f2d_point_hash(struct f2d_point pt, size_t buckets);
