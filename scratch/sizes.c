#include <stdio.h>

struct poor {
        char ch;
        char* s;
        int i;
};

struct good {
        char ch;
        int i;
        char* s;
};

int main(void)
{
        printf("Size of poor struct: %zu\n", sizeof(struct poor));
        printf("Size of good struct: %zu\n", sizeof(struct good));
}

