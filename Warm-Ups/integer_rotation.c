#include <stdio.h>

// a,b,c -> b,c,a
void rotate3(int *a, int *b, int *c) {
    int a_value = *a;
    int b_value = *b;
    int c_value = *c;

    *c = a_value;
    *b = c_value;
    *a = b_value;
}

int main() {
    
    int a = 10;
    int b = -4;
    int c = 30;


    printf("Before rotation: %3d %3d %3d\n", a, b, c);

    rotate3(&a, &b, &c);

    printf("After rotation:  %3d %3d %3d\n", a, b, c);
    
    return 0;
}

