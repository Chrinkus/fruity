#include <stdlib.h>
#include <stdio.h>

#include "fruity.h"

struct location {
	int count;
	char id;
};

void print_loc_cnt(const void** a, int i, int j, void* d)
{
	const struct location** al = (const struct location**)a;
	(void)d;

	printf("%d ", al[i][j].count);
}

void print_loc_id(const void** a, int i, int j, void* d)
{
	const struct location** al = (const struct location**)a;
	(void)d;

	printf("%c ", al[i][j].id);
}

void init_location(void** a, int r, int c, void* d)
{
	struct location** al = (struct location**)a;
	struct location* tmp = (struct location*)d;

	al[r][c] = *tmp;
	/*
	al[r][c] = (struct location){
		.count = 0,
		.id = '.',
	};
	*/
}

typedef void (*row_f)(const void**, int, void*);
typedef void (*col_f)(const void**, int, int, void*);
typedef void (*row_f_mut)(void**, int, void*);
typedef void (*col_f_mut)(void**, int, int, void*);

void foreach(const void** a, int r, int c, row_f rowf, col_f colf, void* data)
{
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j)
			if (colf)
				colf(a, i, j, data);
		if (rowf)
			rowf(a, i, data);
	}
}

void initialize(void** a, int r, int c, row_f_mut rowf, col_f_mut colf, void* data)
{
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j)
			if (colf)
				colf(a, i, j, data);
		if (rowf)
			rowf(a, i, data);
	}
}

void init_inc_ints(void** a, int r, int c, void* d)
{
	int** ai = (int**)a;
	int* p = (int*)d;

	ai[r][c] = (*p)++;
}

void init_inc_chars(void** a, int r, int c, void* d)
{
	char** ac = (char**)a;
	char* p = (char*)d;

	ac[r][c] = (*p)++;
}

void print_int(const void** a, int i, int j, void* d)
{
	const int** ai = (const int**)a;
	(void)d;

	printf("%d ", ai[i][j]);
}

void print_char(const void** a, int i, int j, void* d)
{
	const char** ac = (const char**)a;
	(void)d;

	printf("%c ", ac[i][j]);
}

void print_line(const void**a, int i, void* d)
{
	(void)a;
	(void)i;
	(void)d;
	printf("\n");
}

int main(void)
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	printf("Char 2D array\n\n");
	int rows = 6;
	int cols = 3;
	char** ac;
	fruity_new(char, rows, cols, ac);

	char ch = 'a';
	initialize(fruity_cast_mutable(ac), rows, cols,
			NULL, init_inc_chars, &ch);

	foreach(fruity_cast(ac), rows, cols, print_line, print_char, NULL);

	free(ac);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	printf("\nInt 2D array\n\n");
	rows = 4;
	cols = 5;
	int** ai;
	fruity_new(int, rows, cols, ai);

	int v = 10;
	initialize(fruity_cast_mutable(ai), rows, cols,
			NULL, init_inc_ints, &v);

	foreach(fruity_cast(ai), rows, cols, print_line, print_int, NULL);

	free(ai);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	printf("\nStruct 2D array\n\n");
	rows = 10;
	cols = 20;
	struct location** al;
	fruity_new(struct location, rows, cols, al);

	struct location init = { .count = 0, .id = '.' };
	initialize(fruity_cast_mutable(al), rows, cols,
			NULL, init_location, &init);

	printf("Counts..\n");
	foreach(fruity_cast(al), rows, cols, print_line, print_loc_cnt, NULL);

	printf("IDs..\n");
	foreach(fruity_cast(al), rows, cols, print_line, print_loc_id, NULL);

	free(al);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	return EXIT_SUCCESS;
}

