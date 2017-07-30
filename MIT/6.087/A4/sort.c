#include <stdlib.h>
#include <stdio.h>

#define arr_size(arr) sizeof(arr)/sizeof((arr)[0])

void insertion_sort(int* begin, int* end);
void shift_element(int* begin, int* elem);
void shell_sort(int* begin, int* end);
void shift_element_by_gap(int* begin, int* elem, size_t gap);

int main() {
    int arr[] = {7, 6, 5, 4, 3, 2, 1, 1, 2, 3};
    size_t size = arr_size(arr), i = 0;
    //insertion_sort(arr, arr + size);
    shell_sort(arr, arr + size);
    while(i < size) {
        printf("%d ", arr[i++]);
    }
    printf("\n");
    return 0;
}

void insertion_sort(int* begin, int* end) {
    int* ptr = begin;
    while(ptr != end) {
        shift_element(begin, ptr++);            
    }
}

void shell_sort(int* begin, int* end) {
    size_t gap = (end - begin) / 2;
    while(gap > 0) {
        int* ptr = begin;
        while(ptr < end) {
            shift_element_by_gap(begin, ptr++, gap);            
        }
        gap /= 2;
    }
}

void shift_element_by_gap(int* begin, int* elem, size_t gap) {
    int val = *elem;
    while(elem >= begin + gap && *(elem - gap) > val) {
        *elem = *(elem - gap);
        elem -= gap;
    }
    *elem = val;
}

void shift_element(int* begin, int* elem) {
    int val = *elem;
    while(elem != begin && *(elem - 1) > val) {
        *elem = *(elem - 1);
        --elem;
    }
    *elem = val;
}

