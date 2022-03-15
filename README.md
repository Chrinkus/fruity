# Fruity2D

A C library for creating and working with 2D arrays whose dimensions are not known until run-time.

## Design Goals

* Double-indexed api `arr[i][j]`
* Contiguous data
* Remove the need to hand-write nested for-loops
* Generic array types (int, char, struct custom)
* Const by default, mutable on demand

## Guiding Need

Two-dimensional problems from the Advent of Code will be the final testing ground of this project. I will attempt to document selected solutions and the affects on Fruity's design that they have.

