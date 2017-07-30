#include <stdlib.h>
#include <stdio.h>

void lndsubseq(double *nums, int N, int *start, int *len);

int main() {
    int N, i = 0, start, len;
    //getchar() here is to check that N is indeed an int and not truncated double
    int no_err = scanf("%d", &N) == 1 && getchar() == ' ' && N >= 2;
    double *nums = no_err ? (double*) malloc(sizeof(double) * N) : NULL;
    //if no_err == 0 in the first place, no data will be read via NULL nums ptr
    while(i < N && (no_err = no_err && scanf("%lf", nums + i++) == 1));
    if(no_err) {
        lndsubseq(nums, N, &start, &len);
        if(len > 1) {
            printf("%d %d\n", len, start); 
        } else {
            printf("0\n");
        }
    } else {
        printf("[error]");
    }
    free(nums);
    return 0;
}

void lndsubseq(double *nums, int N, int *start, int *len) {
    *len = 0;
    int cur_start = 0, cur_stop;    
    while(cur_start < N - *len) {
        cur_stop = cur_start;
        while(cur_stop < N - 1 && nums[cur_stop] <= nums[cur_stop + 1]) {
            cur_stop++;
        }
        if(*len < cur_stop - cur_start + 1) {
            *len = cur_stop - cur_start + 1;
            *start = cur_start + 1;
        }
        cur_start = cur_stop + 1;
    }
}
