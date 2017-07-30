#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "trie.h"

struct s_trie_node
{
    char* translation; /* NULL if node have no word */
    struct s_trie_node* children[UCHAR_MAX + 1];
};

/* pointer to the root node of the trie structure */
static struct s_trie_node* proot = NULL;

/* allocate new node on the heap */
struct s_trie_node* new_node(void);

/* deletes node and all its childrean recursively */
void delete_node(struct s_trie_node* node);

void init_trie() 
{
    proot = new_node();
}

struct s_trie_node* new_node(void) 
{
    struct s_trie_node* node = (struct s_trie_node*) malloc(sizeof(struct s_trie_node));
    node->translation = NULL;
    /* important to use sizeof here for portability (pointer size might differ etc.) */
    memset(node->children, 0, sizeof(node->children));
    return node;
}

void delete_trie() 
{
    delete_node(proot);
    proot = NULL;
}

void delete_node(struct s_trie_node* node) 
{
    if(node == NULL)
    {
        return;
    }
    int i = 0;
    while(i <= UCHAR_MAX)
    {
        delete_node(node->children[i++]);
    }
    free(node->translation);
    free(node);
}

int add_word(const char* word, char* translation) 
{
    struct s_trie_node* ptr = proot;
    while(*word && ptr->children[*word])
    {
        ptr = ptr->children[*word++];
    }
    while(*word)
    {
        ptr = ptr->children[*word++] = new_node();
    }
    if(ptr->translation)
    {
        ptr->translation = realloc(ptr->translation, strlen(ptr->translation) + strlen(translation) + 2);
        strcat(strcat(ptr->translation, ","), translation);
        return 0;
    }
    ptr->translation = strdup(translation);
    return 1;
}

char* lookup_word(const char* word) 
{
    struct s_trie_node* ptr = proot;
    while(*word && (ptr = ptr->children[*word++]));
    return ptr ? ptr->translation : NULL;
}
