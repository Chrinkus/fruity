# Fruity v0.4 Intent

* Add infinite grid struct
* Add 2D point struct
* Fix const-correctness in `_adjacent_4`
    - currently takes a mutable fruity\_2d struct, needs to take const
* Revert row and column type change back to integers
    - some row and column operations have negative values applied and applying those to `size_t`s is cumbersome with appropriate warnings turned on
* Scrap struct typedefs, streamline type naming
    - this was a flavour experiment when making the library but I prefer to use the 'struct' keyword
* Rename 'new' to communicate that it is allocating and may fail
    - though this library is separate from 'libcgs' I will attempt to maintain consistent naming conventions between the two. A 'new' function does not fail in 'libcgs' so it should not fail here. However a `fruity_2d` grid is allocated only once at definition and does not change so it doesn't make sense to defer allocation to first usage.
* Add an elegant way to create a new 2D array of the same proportions as another but for different types
    - as I've used 2D arrays I have found it better to keep a const-qualified initial 'input' state and then have a second mutable 'map' for counts, changes or other data.
* Write an io input function to read a 2D character map into a fruity struct
    - this is sorely needed as the code is exactly the same every time

## Infinite 2D Planes

Use a hash set of points or a hash map with point keys and generic values.

## Library Naming Considerations

With the addition of infinite structures, the library is growing too big to prefix everything with `fruity`. Even without extra structs, the future consideration of what to do with 3D data sets suggests `fruity_2d` and `fruity_3d`.

The solution is to shorten the prefix to `f2d` and `f3d`.

Now that we have extra structures we need to explicitly name the default 2D bound grid. So how about `f2d_grid`? Does that expand in 3D, does `f3d_grid` make sense? Hopefully.

```
struct f2d_grid;
struct f3d_grid;

struct f2d_set;
struct f2d_table;

struct f2d_point;
struct f3d_point;
```

That all looks pretty good and consistent!

Speaking of points, they will need to get the full header/source/test treatment.

