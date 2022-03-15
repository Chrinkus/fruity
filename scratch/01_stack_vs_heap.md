# Stack vs Heap

## Uninitialized Arrays

When the dimensions of a 2D array are known at compile-time, creation on the stack is trivial:

```
// Inside a function
int a[5][3];
```

This creates an un-initialized block of memory on the stack similar to the following:
```
[garbage][garbage][garbage]
[garbage][garbage][garbage]
[garbage][garbage][garbage]
[garbage][garbage][garbage]
[garbage][garbage][garbage]
```

When the dimensions of a 2D array are NOT known at compile-time the creation of a data structure can be tricky. Fruity will alleviate this concern and help to minimize code:

```
int** a;
// These dimensions may come from user input, derived from data, etc.
int rows = 5;
int cols = 3;

fruity_new(int, rows, cols, a);
```

This creates a block of memory on the heap similar to the above stack example. The problem now becomes how to initialize the space in the array.

## Initialized arrays

An improvement would be to initialize the 2D space:
```
int a[5][3] = { 0 };
```

Resulting in the (much cleaner) arrangement:
```
[0][0][0]
[0][0][0]
[0][0][0]
[0][0][0]
[0][0][0]
```
