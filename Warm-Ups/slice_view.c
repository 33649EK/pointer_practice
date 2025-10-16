#include <stdio.h>


struct view {int *begin, *end;};

int sum(struct view v) {
    if (v.begin == v.end) 
        return 0;

    return *v.begin + *(v.end - 1);
}

int sum_range(struct view v) {
    int sum = 0;
    for (int *p = v.begin; p != v.end; ++p) {
        sum += *p;
    }
    return sum;
}

int main() {

    int arr[] = {1, 2, 3, 4, 17};

    struct view v = {arr+0, arr + 5};

    printf("Sum: %d\n", sum(v));

    printf("Sum of full range: %d\n", sum_range(v));

    return 0;
}