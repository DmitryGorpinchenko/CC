#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 255
#define MAX_LEN     10
#define BRANCHING   2

struct node
{
    struct node* children[BRANCHING]; /*used when in tree*/  
    struct node* parent;              /*used when in tree*/
    struct node* next;                /*used when in list*/ 
    float  freq;
    char   symbol;                    /*nonzero symbol <=> leaf node*/
};

char   code[MAX_SYMBOLS][MAX_LEN];
struct node* root  = NULL; /*tree of symbols*/
struct node* qhead = NULL; /*list of current symbols*/

struct node* nalloc(char symbol, float freq)
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


void freetree(struct node* root)
{
	if(root == NULL)
	{
		return;
	}
	for(int i = 0; i < BRANCHING; i++) 
	{
	    freetree(root->children[i]);
	}
	free(root);
}

int is_leaf(struct node* node) 
{
    return node->symbol;
}

/*
 *  inserts an element into the priority queue
 *  NOTE: makes use of global variable qhead
 */
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

/*
 *  removes the first element
 *  NOTE: makes use of global variable qhead
 */
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

/*
 *  generates the string codes given the tree
 *	NOTE: makes use of the global variable root
 */
void generate_code(struct node* root, int depth)
{
	char symbol;
	int  len; /*length of code*/
	
	if(is_leaf(root))
	{
		symbol = root->symbol;
		len    = depth;
		/*start backwards*/
		code[symbol][len] = 0;
		while(--len >= 0)
		{
		    for(int i = 0; i < BRANCHING; i++)
		    {
		        if(root->parent->children[i] == root) 
		        {
		            code[symbol][len] = i + '0';
		        }
		    }
		    root = root->parent;
		}   
	}
	else
	{   
	    for(int i = 0; i < BRANCHING; i++) 
	    {
	        generate_code(root->children[i], depth + 1);    
	    }
	}
}

/*
 *	output code file
 */
void dump_code(FILE* fp)
{
	int i;
	for(i = 0; i < MAX_SYMBOLS; i++)
	{
	    /*non empty*/
		if(code[i][0]) 
		{
			fprintf(fp, "%c %s\n", i, code[i]);
        }
	}
}

/*
 *	outputs compressed stream
 */
void encode(char* str, FILE* fout)
{
	while(*str)
	{
		fprintf(fout, "%s", code[*str]);
		str++;
	}
}

int main()
{
    struct node* p         = NULL;
    struct node* lc, *rc;
    float        freq[]    = {0.01, 0.04, 0.05, 0.11, 0.19, 0.20, 0.4};
	int          NCHAR     = 7; /*number of characters*/
    int          i         = 0;
	const char*  CODE_FILE = "code.txt";
	const char*  OUT_FILE  = "encoded.txt";
	FILE*        fout      = NULL;
	
	memset(code, 0, sizeof(code));
    
    /*test pq*/
    pq_insert(nalloc('a',  0.1));
    pq_insert(nalloc('b',  0.2));
    pq_insert(nalloc('c', 0.15));
    /*making sure it pops in the right order*/
	while(p = pq_pop())
    {
        free(p);
    }
	
	qhead = NULL;
    for(i = 0; i < NCHAR; i++)
    {
        pq_insert(nalloc('a' + i, freq[i]));
    }
    /*build tree*/
    for(i = 0; i < NCHAR - 1; i++)
    {
        lc = pq_pop();
        rc = pq_pop();
        p = nalloc(0, lc->freq + rc->freq);
        lc->parent = rc->parent = p;
        p->children[1] = rc; p->children[0] = lc;
        pq_insert(p);
    }
    root = pq_pop();
	generate_code(root, 0);
	puts("code:");
	fout = fopen(CODE_FILE, "w");
	dump_code(stdout);
	dump_code(fout);
	fclose(fout);

	puts("orginal: abbacafebad");
	fout = fopen(OUT_FILE, "w");
	encode("abbacafebad", stdout);
	encode("abbacafebad", fout);
	fclose(fout);
	freetree(root);
    return 0;
}
