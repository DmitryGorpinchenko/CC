#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define BASE 10

uint64_t triang_num_id(uint64_t a_n);

int main() {
    uint64_t a_n;
    char *in = NULL, c;
    size_t size;
    getline(&in, &size, stdin); 
    printf("%ju\n", sscanf(in, "%ju %c", &a_n, &c) == 1 ? triang_num_id(a_n) : 0); 
    free(in);
    return 0;
}

uint64_t triang_num_id(uint64_t a_n) {
    if(a_n > LLONG_MAX)  {
        return 0;
    }
    uint64_t lo = 1, hi = a_n;
    while(lo <= hi) { 
        uint64_t mid = lo + (hi - lo) / 2;
        if((a_n * 2) % (mid + 1) == 0 && a_n * 2 / (mid + 1) == mid) {
            return mid;
        } else if(a_n * 2 / (mid + 1) / mid >= 1) {
        // '>=' used instead of just '>' due to the integer division issues
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return 0;
}
