#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

#define SIZE_RING 32
typedef int e_v;
#include "ring.inc"

int main()
{
    int i, n;
    init_ring();
    for (i = 0; i < 8; i++) {
        if (!rngfull()) {
            rngput(i);
        }
    }
    while (!rngempty()) {
        n = rngget();
        printf("n=%d\n", n);
    }
    return 0;
}
