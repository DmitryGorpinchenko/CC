#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hash.h"

#define MAX_WORD_LEN 1024

int greater_than(uint64_t num, uint64_t thresh);

int main(int argc, char* argv[])
{
    FILE* fp = fopen("book.txt", "r");
    char word[MAX_WORD_LEN];
    
    init_hashtable();
    while(fscanf(fp, "%s", word) == 1)
    {
        lookup(word, CREATE)->count++;
    }
    fclose(fp);
    
    print_if(greater_than, 100);
    clear_table();
    return 0;
}

int greater_than(uint64_t num, uint64_t thresh) 
{
    return num > thresh;
}
