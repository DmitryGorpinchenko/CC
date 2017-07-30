#include <stdio.h>
#include <stdlib.h>

int x1(int n);
int x2(int n);
int conv(int (*x1)(int), int (*x2)(int), int n, int s, int e);

int main() {
    int ids[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    for(int i = 0; i < 10; i++) {
        printf("%d ", conv(x1, x2, ids[i], 0, 4));
        printf("%d ", conv(x2, x1, ids[i], 0, 4));
    }
    printf("\n");
    return 0;
} 

int x1(int n) {
    return n >= 0 && n < 4 ? 4 : 0;
}

int x2(int n) {
    if(n >= 4) {
        return 3;
    } else if(n >= 0) {
        return n;
    }
    return 0;
}

int conv(int (*x1)(int), int (*x2)(int), int n, int s, int e) {
    int res = 0;
    for(int k = s; k < e; k++) {
        res += x1(k) * x2(n - k);
    }
    return res;
}
