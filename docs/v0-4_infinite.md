# Infinite 2D Plane

When the boundaries of a 2-dimensional space are unknown or are not trivially discerned an infinte plane is required.

## Data Structure

The structure that I will use to implement an infinte plane are a hash-set and a hash-table. Though I may regret this later, for brevity I will just be naming them `f2d_set` and `f2d_table`.

## Programming Notes

For this implementation I will be eschewing the "test-every-function" approach and will instead just be testing the "end-to-end" usage of the structure. In order to prove the effectiveness of the internal functions (collision behaviour, re-hashing effectiveness) I'll need to learn how to profile my code.

The impetus for this decision has been the desire to get away from private headers for testing internal functions in favour of using static functions in source files. This decision comes after [taking the question to Reddit](https://www.reddit.com/r/C_Programming/comments/102jjif/private_headers_or_static_functions/) and reviewing the various answers.
