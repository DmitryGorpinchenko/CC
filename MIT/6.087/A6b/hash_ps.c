#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#define MAX_BUCKETS 100000
#define MULTIPLIER      31

struct wordrec* walloc(const char* str);
size_t hashstring(const char* str);

/* hash table buckets */
static struct wordrec* table[MAX_BUCKETS];

void init_hashtable() 
{
    memset(table, 0, sizeof(table));
}

struct wordrec* walloc(const char* str)
{
    struct wordrec* p = (struct wordrec*) malloc(sizeof(struct wordrec));
    if(p != NULL)
    {
        p->count = 0;
        p->word = strdup(str); /* creates a duplicate */
        p->next = NULL;
    }
    return p;
}

size_t hashstring(const char* str)
{
    uint64_t hash = 0;
    while(*str)
    {
        hash = hash * MULTIPLIER + *str++;
    }
    return hash % MAX_BUCKETS;
}

/* returns a pointer to the word or creates it if required */
struct wordrec* lookup(const char* str, int create)
{
    size_t hash = hashstring(str);
    struct wordrec* ptr = table[hash];
    while(ptr != NULL && strcmp(ptr->word, str) != 0) 
    {
        ptr = ptr->next;
    }
    if(ptr == NULL && create) 
    {
        struct wordrec* tmp = walloc(str);
        tmp->next = table[hash];
        table[hash] = tmp;    
        return table[hash];    
    }
    return ptr;
}

void print_if(int (*fp)(uint64_t count, uint64_t thresh), uint64_t thresh) 
{
    int i = 0;
    struct wordrec* wp = NULL;   
    for(i = 0; i < MAX_BUCKETS; i++)
    {
        for(wp = table[i]; wp != NULL; wp = wp->next)
        {
            if(fp(wp->count, thresh))
            {
                printf("%s-->%ld\n", wp->word, wp->count);
            }
        }
    }
}

void clear_table()
{
    int i = 0; 
    struct wordrec* wp = NULL, *tmp = NULL;  
    while(i < MAX_BUCKETS)
    {
        wp = table[i++];
        while((tmp = wp) != NULL)
        {
            wp = wp->next;
            free(tmp->word);
            free(tmp); 
        }
    }
}
