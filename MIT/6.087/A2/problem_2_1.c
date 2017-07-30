#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

int main() {
    printf("*** 64bit machine ***\n\n");
    printf("char:          size: %2lu | MIN_VAL: %20d | MAX_VAL: %20d\n", sizeof(char), SCHAR_MIN, SCHAR_MAX);
    printf("unsigned char: size: %2lu | MIN_VAL: %20u | MAX_VAL: %20u\n", sizeof(unsigned char), 0, UCHAR_MAX);
    printf("short:         size: %2lu | MIN_VAL: %20d | MAX_VAL: %20d\n", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("int:           size: %2lu | MIN_VAL: %20d | MAX_VAL: %20d\n", sizeof(int), INT_MIN, INT_MAX);
    printf("unsigned int:  size: %2lu | MIN_VAL: %20u | MAX_VAL: %20u\n", sizeof(unsigned int), 0, UINT_MAX);
    printf("unsigned long: size: %2lu | MIN_VAL: %20u | MAX_VAL: %20lu\n", sizeof(unsigned long), 0, ULONG_MAX);
    printf("float:         size: %2lu | MIN_VAL: %20e | MAX_VAL: %20e\n", sizeof(float), FLT_MIN, FLT_MAX);
    return 0;
}
