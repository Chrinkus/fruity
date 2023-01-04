/* fruity_set.h
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

#include "fruity_point.h"

struct f2d_set_bucket;

struct f2d_set {
        size_t                  length;
        struct f2d_set_bucket** table;
        size_t                  capacity;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Set Inline Getters
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

inline size_t
f2d_set_length(const struct f2d_set* fs)
{
        return fs->length;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Set Operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * f2d_set_new
 *
 * Construct an empty f2d_set and return it. Does not allocate.
 *
 * @return      An empty f2d_set.
 */
struct f2d_set
f2d_set_new(void);

/**
 * f2d_set_free
 *
 * Free the contents of an f2d_set.
 *
 * @param p     A void-pointer to an f2d_set.
 */
void
f2d_set_free(void* p);

/**
 * f2d_set_lookup
 *
 * Search a set for a point.
 *
 * @param fs    A read-only pointer to an f2d_set to search.
 * @param pt    A read-only pointer to a point that we are looking for.
 *
 * @return      A read-only pointer to the set element if found, NULL if point
 *              does not exist in the set.
 */
const struct f2d_point*
f2d_set_lookup(const struct f2d_set* fs, const struct f2d_point* pt);

/**
 * f2d_set_insert
 *
 * Insert a point into a set. Does nothing if point already exists in set.
 *
 * @param fs    A mutable pointer to the set to insert into.
 * @param pt    A read-only pointer to a point that we want to insert into the
 *              set.
 *
 * @return      A read-only pointer to the set element if it already exists or
 *              was successfully inserted. NULL on insertion error.
 */
const struct f2d_point*
f2d_set_insert(struct f2d_set* fs, const struct f2d_point* pt);

/**
 * f2d_set_remove
 *
 * Remove a point from a set. Does nothing if point does not exist in set.
 *
 * @param fs    A mutable pointer to the set to remove a point from.
 * @param pt    A read-only pointer to a point that we want to remove from the
 *              set.
 */
void
f2d_set_remove(struct f2d_set* fs, const struct f2d_point* pt);
