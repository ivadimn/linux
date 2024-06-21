#include "solution.h"
#include <stdio.h>

int main() {

    int count = 11;
    int len = stringStat("Vasia", 10, &count);
    printf("len = %d, count = %d\n", len, count);
    return 0;
}

