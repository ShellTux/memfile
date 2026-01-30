#include <stdio.h>
#include <stddef.h>

const size_t factorial(const size_t number) {
    if (number <= 1) {
        return 1;
    }

    return number * factorial(number - 1);
}

int main(void) {
    for (size_t n = 0; n <= 30; n++) {
        printf("%zu! = %zu\n", n, factorial(n));
    }

    return 0;
}
