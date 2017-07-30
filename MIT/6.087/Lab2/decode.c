#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN   10
#define BRANCHING 2

struct tnode
{
    struct tnode* children[BRANCHING];
    int    isleaf;
    char   symbol;
};

struct tnode* root = NULL;

struct tnode* talloc()
{
    struct tnode* p = (struct tnode*) malloc(sizeof(struct tnode));
    
    if(p != NULL)
    {
        for(int i = 0; i < BRANCHING; i++) 
        {   
            p->children[i] = NULL;
        }
        p->symbol = 0;
		p->isleaf = 0;
    }
    return p;
}

/*
    builds the symbol tree given the list of symbols and codes
	NOTE: alters the global variable root that has already been allocated in main
*/
void build_tree(FILE* fp)
{
	char   symbol;
	char   strcode[MAX_LEN];
	int    items_read;
	int	   i, len, bit;
	struct tnode* curr = NULL;

	while(!feof(fp))
	{
		items_read = fscanf(fp,"%c %s\n", &symbol, strcode);
		if(items_read != 2) 
		{
		    break;
	    }
		curr = root;
		len = strlen(strcode);
		for(i = 0; i < len; i++)
		{
		    bit = strcode[i] - '0';
			if(curr->children[bit] == NULL) 
			{
			    curr->children[bit] = talloc();
			}
		    curr = curr->children[bit];
		}
		curr->isleaf = 1;
		curr->symbol = symbol;
	}
}

void decode(FILE* fin, FILE* fout)
{
	char   c;
	struct tnode* curr = root;
	
	while((c = getc(fin)) != EOF)
	{
		if((curr = curr->children[c - '0'])->isleaf) 
		{
		    putc(curr->symbol, fout);
		    curr = root;
		}
	}
}

void freetree(struct tnode* root)
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

int main()
{
	const char* IN_FILE   = "encoded.txt";
	const char* CODE_FILE = "code.txt";
	const char* OUT_FILE  = "decoded.txt";
	FILE* fout;
	FILE* fin;
	
	fin = fopen(CODE_FILE, "r");
	root = talloc();
	build_tree(fin);
	fclose(fin);

	fin  = fopen(IN_FILE,  "r");
	fout = fopen(OUT_FILE, "w");
	decode(fin, fout);
	fclose(fin);
	fclose(fout);

	freetree(root);
	return 0;
}

