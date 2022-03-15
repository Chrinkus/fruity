#include <stdlib.h>
#include <stdio.h>

#include "fruity.h"

struct location {
	int count;
	char id;
};

void print_loc_cnt(FruityConst2D a, int i, int j, void* d)
{
	const struct location** al = (const struct location**)a;
	(void)d;

	printf("%d ", al[i][j].count);
}

void print_loc_id(FruityConst2D a, int i, int j, void* d)
{
	const struct location** al = (const struct location**)a;
	(void)d;

	printf("%c ", al[i][j].id);
}

void init_location(FruityMutable2D a, int r, int c, void* d)
{
	struct location** al = (struct location**)a;
	struct location* tmp = (struct location*)d;

	al[r][c] = *tmp;
}

void init_inc_ints(FruityMutable2D a, int r, int c, void* d)
{
	int** ai = (int**)a;
	int* p = (int*)d;

	ai[r][c] = (*p)++;
}

void init_inc_chars(FruityMutable2D a, int r, int c, void* d)
{
	char** ac = (char**)a;
	char* p = (char*)d;

	ac[r][c] = (*p)++;
}

void print_int(FruityConst2D a, int i, int j, void* d)
{
	const int** ai = (const int**)a;
	(void)d;

	printf("%d ", ai[i][j]);
}

void print_char(FruityConst2D a, int i, int j, void* d)
{
	const char** ac = (const char**)a;
	(void)d;

	printf("%c ", ac[i][j]);
}

void print_line(FruityConst2D a, int i, void* d)
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
	fruity_transform(fruity_cast_mutable(ac), rows, cols,
			NULL, init_inc_chars, &ch);

	fruity_foreach(fruity_cast(ac), rows, cols, print_line, print_char, NULL);

	free(ac);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	printf("\nInt 2D array\n\n");
	rows = 4;
	cols = 5;
	int** ai;
	fruity_new(int, rows, cols, ai);

	int v = 10;
	fruity_transform(fruity_cast_mutable(ai), rows, cols,
			NULL, init_inc_ints, &v);

	fruity_foreach(fruity_cast(ai), rows, cols, print_line, print_int, NULL);

	free(ai);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	printf("\nStruct 2D array\n\n");
	rows = 10;
	cols = 20;
	struct location** al;
	fruity_new(struct location, rows, cols, al);

	struct location init = { .count = 0, .id = '.' };
	fruity_transform(fruity_cast_mutable(al), rows, cols,
			NULL, init_location, &init);

	printf("Counts..\n");
	fruity_foreach(fruity_cast(al), rows, cols, print_line, print_loc_cnt, NULL);

	printf("IDs..\n");
	fruity_foreach(fruity_cast(al), rows, cols, print_line, print_loc_id, NULL);

	free(al);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
	return EXIT_SUCCESS;
}

