#include <stdlib.h>
#include <stdio.h>

#include "fruity.h"

struct location {
	int count;
	char id;
};

typedef void (*row_f)(const void**, int);
typedef void (*col_f)(const void**, int, int);

void foreach(const void** a, int r, int c, row_f rowf, col_f colf)
{
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j)
			if (colf)
				colf(a, i, j);
		if (rowf)
			rowf(a, i);
	}
}

void print_int(const void** a, int i, int j)
{
	const int** ai = (const int**)a;

	printf("%d ", ai[i][j]);
}

void print_line(const void**a, int i)
{
	(void)a;
	(void)i;
	printf("\n");
}

int main(void)
{
	printf("Char 2D array\n\n");
	int rows = 6;
	int cols = 3;
	char** ac;
	fruity_new(char, rows, cols, ac);

	char ch = 'a';
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j, ++ch)
			ac[i][j] = ch;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			printf("%c ", ac[i][j]);
		printf("\n");
	}

	printf("\nInt 2D array\n\n");
	rows = 4;
	cols = 5;
	int** ai;
	fruity_new(int, rows, cols, ai);

	int v = 10;
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j, ++v)
			ai[i][j] = v;

	foreach((const void**)ai, rows, cols, print_line, print_int);

	printf("\nStruct 2D array\n\n");
	rows = 10;
	cols = 20;
	struct location** al;
	fruity_new(struct location, rows, cols, al);

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j) {
			al[i][j] = (struct location){
				.count = 0,
				.id = '.',
			};
		}

	printf("Counts..\n");
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			printf("%d ", al[i][j].count);
		printf("\n");
	}

	printf("IDs..\n");
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			printf("%c ", al[i][j].id);
		printf("\n");
	}

	free(ac);
	free(ai);
	free(al);

	return EXIT_SUCCESS;
}

