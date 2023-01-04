/* fruity_set.c
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

#include "fruity_set.h"

#include <stdlib.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Constants
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

enum set_magic {
        F2D_SET_DEFAULT_CAPACITY = 31,
};

const double F2D_SET_DEFAULT_LOAD_FACTOR = 0.8;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Private Types
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct f2d_set_bucket {
        struct f2d_point        pt;
        struct f2d_set_bucket*  next;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Static functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

static struct f2d_set_bucket*
set_bucket_alloc(const struct f2d_point* pt)
{
        struct f2d_set_bucket* b = malloc(sizeof(struct f2d_set_bucket));
        if (!b)
                return NULL;
        b->pt = *pt;
        b->next = NULL;
        return b;
}

static void
set_bucket_free(void* p)
{
        if (!p)
                return;

        struct f2d_set_bucket* b = p;
        set_bucket_free(b->next);
        free(b);
}

// 'is_prime()' and 'next_prime()' adapted from
// Wikipedia article "Primality Test":
// https://en.wikipedia.org/wiki/Primality_test
enum prime_magic { PRIME_MAGIC = 6 };

static int
is_prime(size_t n)
{
        if (n == 2 || n == 3)
                return 1;

        if (n <= 1 || n % 2 == 0 || n % 3 == 0)
                return 0;

        for (size_t i = 5; i * i <= n; i += PRIME_MAGIC)
                if (n % i == 0 || n % (i + 2) == 0)
                        return 0;

        return 1;
}

static size_t
next_prime(size_t n)
{
        n += n % 2 == 0 ? 1 : 2;

        if (is_prime(n))
                return n;

        n = (n / PRIME_MAGIC + 1) * PRIME_MAGIC;

        for ( ; 1; n += PRIME_MAGIC) {
                if (is_prime(n - 1))
                        return n - 1;
                if (is_prime(n + 1))
                        return n + 1;
        }
}

static size_t
get_new_cap(const struct f2d_set* fs)
{
        return fs->capacity == 0
                ? F2D_SET_DEFAULT_CAPACITY
                : next_prime(fs->capacity * 2);
}

static void*
set_grow(struct f2d_set* fs)
{
        size_t new_cap = get_new_cap(fs);
        struct f2d_set_bucket** new_tab =
                malloc(new_cap * sizeof(struct f2d_set_bucket));
        if (!new_tab)
                return NULL;

        for (size_t i = 0; i < new_cap; ++i)
                new_tab[i] = NULL;

        // rehash
        for (size_t i = 0; i < fs->capacity; ++i) {
                struct f2d_set_bucket* bp = fs->table[i];
                while (bp) {
                        struct f2d_set_bucket* tmp = bp->next;
                        size_t hash = f2d_point_hash(bp->pt, new_cap);
                        bp->next = new_tab[hash];
                        new_tab[hash] = bp;
                        bp = tmp;
                }
        }

        free(fs->table);
        fs->table = new_tab;
        fs->capacity = new_cap;
        return fs;
}

static double
get_load_factor(const struct f2d_set* fs)
{
        return (double)fs->length / (double)fs->capacity;
}

static void*
check_load_factor(struct f2d_set* fs)
{
        if (get_load_factor(fs) > F2D_SET_DEFAULT_LOAD_FACTOR)
                return set_grow(fs);
        return fs;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Public functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

struct f2d_set
f2d_set_new(void)
{
        return (struct f2d_set){
                .length = 0,
                .table = NULL,
                .capacity = 0,
        };
}

void
f2d_set_free(void* p)
{
        if (!p)
                return;

        struct f2d_set* fs = p;
        if (!fs->table)
                return;

        for (size_t i = 0; i < fs->capacity; ++i)
                if (fs->table[i])
                        set_bucket_free(fs->table[i]);

        free(fs->table);
        fs->length = 0;
        fs->capacity = 0;
}

const struct f2d_point*
f2d_set_lookup(const struct f2d_set* fs, const struct f2d_point* pt)
{
        if (!fs->table)
                return NULL;

        size_t hashval = f2d_point_hash(*pt, fs->capacity);
        for (const struct f2d_set_bucket* b = fs->table[hashval];
                        b;
                        b = b->next)
                if (f2d_point_cmp(pt, &b->pt) == 0)
                        return &b->pt;
        return NULL;
}

const struct f2d_point*
f2d_set_insert(struct f2d_set* fs, const struct f2d_point* pt)
{
        // check if new, empty hashset and allocate if so
        if (!fs->table && !set_grow(fs))
                return NULL;

        const size_t hashval = f2d_point_hash(*pt, fs->capacity);
        struct f2d_set_bucket* p = fs->table[hashval];

        // check if point already exists in set and return if found
        if (p)
                for (struct f2d_set_bucket* tmp = p; tmp; tmp = tmp->next)
                        if (f2d_point_cmp(pt, &tmp->pt) == 0)
                                return &tmp->pt;

        // not found, allocate new bucket and insert!
        struct f2d_set_bucket* bp = set_bucket_alloc(pt);
        if (!bp)
                return NULL;
        bp->next = p;
        fs->table[hashval] = bp;
        ++fs->length;

        if (!check_load_factor(fs))
                return NULL;

        return &bp->pt;
}

void
f2d_set_remove(struct f2d_set* fs, const struct f2d_point* pt)
{
        size_t hashval = f2d_point_hash(*pt, fs->capacity);

        struct f2d_set_bucket* b = fs->table[hashval];
        if (!b)
                return;         // no buckets at hash, element not in table

        struct f2d_set_bucket* parent = NULL;
        for ( ; b; parent = b, b = b->next)
                if (f2d_point_cmp(pt, &b->pt) == 0)
                        break;  // b now points to element to remove

        if (!b)
                return;         // there were buckets but no match!

        if (parent)
                parent->next = b->next;
        else
                fs->table[hashval] = b->next;

        --fs->length;
        b->next = NULL;         // set_bucket_free is recursive, cut list!
        set_bucket_free(b);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Inline Symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t
f2d_set_length(const struct f2d_set* fs);
