#include <stdlib.h>
#include <stdio.h>

void double_them(int** arr, int rows, int cols)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			arr[i][j] *= 2;
}

typedef void (*op)(const int*);

void print_them(const int* n)
{
	printf("%d ", *n);
}

void foreach(int** arr, int rows, int cols, op func)
{
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			func(&arr[i][j]);
}

int main(void)
{
	// Arbitrarily sized dimensions. These are 'set' in this example but
	// could be determined at run-time.
	const int rows = 7;
	const int cols = 6;

	// Size of the allocation is the elements plus a pointer to each row
	const int size_elements = sizeof(int) * rows * cols;
	const int size_pointers = sizeof(int*) * rows;
	const int size = size_pointers + size_elements;

	// The array
	int** a = malloc(size);

	// A pointer to the first element row
	int* p = (int*)(a + rows);

	// Initialize pointers to rows
	for (int i = 0; i < rows; ++i)
		a[i] = (p + i * cols);

	// Now the array may be double-indexed

	// Set each element to a value
	for (int i = 0, v = 1; i < rows; ++i)
		for (int j = 0; j < cols; ++j, ++v)
			a[i][j] = v;

	// Pass to a function
	double_them(a, rows, cols);

	// Possible foreach implementation
	foreach(a, rows, cols, print_them);

	printf("\n");

	free(a);

	return EXIT_SUCCESS;
}

