#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BITS_PER_SYMBOL 8
#define MAX_SYMBOLS     256
#define MAX_LEN         25
#define BRANCHING       2

struct node
{
    struct node* children[BRANCHING]; /* used when in tree */  
    struct node* parent;              /* used when in tree */
    struct node* next;                /* used when in list */ 
    uint64_t     freq;                /* number of occurances of symbol */  
    uint8_t      symbol;              /* nonzero symbol <=> leaf node */
};

static uint8_t      code[MAX_SYMBOLS][MAX_LEN];
static uint64_t     freq[MAX_SYMBOLS];
static struct node* root  = NULL; /* tree of symbols */
static struct node* qhead = NULL; /* list of current (meta-)symbols */

struct node* nalloc(uint8_t symbol, uint64_t freq);
int          is_leaf(struct node* node); 
void         pq_insert(struct node* p);
struct node* pq_pop();
void         build_tree();
void         precompute_codes(struct node* root, int depth);
void         free_tree(struct node* root);

void init_huffman()
{
    memset(code, 0, sizeof(code));
    memset(freq, 0, sizeof(freq));
    root  = NULL;
    qhead = NULL;
}

void encode(FILE* fin, FILE* fout)
{
    int c;

    while((c = getc(fin)) != EOF) 
    {
        freq[c]++;   
    }
    
    build_tree();
    precompute_codes(root, 0);
    
    rewind(fin);
    while((c = getc(fin)) != EOF)
	{
		fprintf(fout, "%s", code[c]);
	}       
}

void decode(FILE* fin, FILE* fout)
{
    int          c;
	struct node* curr = root;
	
	while((c = getc(fin)) != EOF)
	{
		if(is_leaf(curr = curr->children[c - '0'])) 
		{
		    putc(curr->symbol, fout);
		    curr = root;
		}
	}
}

void dump_code(FILE* fp) 
{
    int i;
    
	for(i = 0; i < MAX_SYMBOLS; i++)
	{
		if(code[i][0]) 
		{
			fprintf(fp, "%c\t%s\n", i, code[i]);
        }
	}
}

double compression_ratio()
{   
    uint64_t total = 0;
    int      i;
    double   avg_code_len = 0.0;
    
    for(i = 0; i < MAX_SYMBOLS; i++)
    {
        total += freq[i];
    }
    for(i = 0; i < MAX_SYMBOLS; i++)
    {
        avg_code_len += (1.0 * strlen(code[i]) * freq[i] / total);
    }
    return BITS_PER_SYMBOL / avg_code_len;
}   

void free_huffman() 
{
    free_tree(root);       
}

struct node* nalloc(uint8_t symbol, uint64_t freq)
{
    struct node* p = (struct node*) malloc(sizeof(struct node));
    
    if(p != NULL)
    {
        for(int i = 0; i < BRANCHING; i++)
        {
            p->children[i] = NULL;
        }
        p->parent = p->next = NULL;
        p->symbol = symbol;
        p->freq   = freq;
    }
    return p;
}

int is_leaf(struct node* node) 
{
    return node->symbol;
}

void build_tree()
{
    struct node* children[BRANCHING];
    struct node* p;
    uint64_t     f;
    int          i;
    
    for(i = 0; i < MAX_SYMBOLS; i++)
    {
        if(freq[i] > 0)
        {
            pq_insert(nalloc(i, freq[i]));
        }
    }               
    //until there is only one element in pq
    while(qhead->next)
    {
        f = 0;
        for(i = 0; i < BRANCHING; i++)
        {
            children[i] = pq_pop();
            f += children[i]->freq;
        }
        p = nalloc(0, f);
        for(i = 0; i < BRANCHING; i++)
        {
            children[i]->parent = p;
            p->children[i] = children[i];
        }
        pq_insert(p);
    }
    root = pq_pop();
}

void precompute_codes(struct node* root, int depth)
{
	uint8_t symbol;
	int     i, len;
	
	if(is_leaf(root))
	{
		symbol = root->symbol;
		len    = depth;
		code[symbol][len] = 0;
		while(--len >= 0)
		{
		    for(i = 0; i < BRANCHING; i++)
		    {
		        if(root->parent->children[i] == root) 
		        {
		            code[symbol][len] = i + '0';
		            break;
		        }
		    }
		    root = root->parent;
		}   
	}
	else
	{   
	    for(i = 0; i < BRANCHING; i++) 
	    {
	        precompute_codes(root->children[i], depth + 1);    
	    }
	}
}

void pq_insert(struct node* p)
{
    struct node* curr = NULL;
    struct node* prev = NULL;
    
    for(curr = qhead; curr && curr->freq < p->freq; curr = curr->next)
    {
        prev = curr;
    }
    p->next = curr;
    if(curr == qhead)
    {
     	qhead = p;
    }
    else
    {
 	    prev->next = p; 
    }
}

struct node* pq_pop()
{
    struct node* p = NULL;
    
    if(p = qhead)
    {
        qhead = qhead->next;
        p->next = NULL;
	}
    return p;
}

void free_tree(struct node* root)
{
    int i;
    
	if(root == NULL)
	{
		return;
	}
	for(i = 0; i < BRANCHING; i++) 
	{
	    free_tree(root->children[i]);
	}
	free(root);
}

