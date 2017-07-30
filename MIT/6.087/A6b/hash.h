#ifndef HASH_H
#define HASH_H

#include <stdint.h>

#define CREATE 1

struct wordrec
{
    char* word;
    uint64_t count;
    struct wordrec* next;
};

void init_hashtable();
struct wordrec* lookup(const char* str, int create);
void print_if(int (*fp)(uint64_t count, uint64_t thresh), uint64_t thresh);
void clear_table();

#endif
