#include <stdlib.h>
#include <stdio.h>

enum { ROWS = 3, COLS = 4 };

//void print_array(int cols, int a[][cols], int rows)
//void print_array(int a[ROWS][COLS])
void print_array(int a[][4], int rows, int cols)
{
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j)
			printf("%2d ", a[i][j]);
		printf("\n");
	}
}

int main(void)
{
	int ai[3][4];
	
	int v = 1;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			ai[i][j] = v++;

	print_array(ai, 3, 4);

	return EXIT_SUCCESS;
}

