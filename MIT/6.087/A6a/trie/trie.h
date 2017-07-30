#ifndef TRIE_H
#define TRIE_H

struct s_trie_node;

/* initializes an empty trie */
void init_trie();

/* deletes trie */
void delete_trie();

/* add word to trie, with translation
   output: non-zero if new node added, zero otherwise */
int add_word(const char* word, char* translation);

/* search trie structure for word and 
   return translations or NULL if not found */
char* lookup_word(const char* word);

#endif
