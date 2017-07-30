#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

enum state {SPACES, SYMBOLS};

void process_file(FILE* file, uint64_t* nc, uint64_t* nw, uint64_t* nl);

int main(int argc, char* argv[])
{   
    /*count of words, lines, characters*/
    uint64_t nw, nl, nc;
    if (argc == 1) 
    {
        process_file(stdin, &nc, &nw, &nl);
        printf("Lines: %lu Words: %lu\n", nl, nw);
        return 0; 
    }
    
    FILE* fp = NULL;
    int   nfiles = --argc; /*ignore the name of the program itself*/
    int   argidx = 1;      
    
    while (nfiles-- > 0)
    {
        if ((fp = fopen(argv[argidx], "r")) == NULL)
        {
            fprintf(stderr, "Unable to open input\n");
            exit(-1);
        }
        process_file(fp, &nc, &nw, &nl);
        printf("Lines: %lu Words: %lu in %s\n", nl, nw, argv[argidx++]);
    }
	return 0;
}

void process_file(FILE* fp, uint64_t* nc, uint64_t* nw, uint64_t* nl) 
{
    char c;
    *nc = *nw = *nl = 0;
    enum state curr_state = SPACES;
    while ((c = getc(fp)) != EOF)
    {
        if (c == '\n')
        {
            ++(*nl);
        }
        if (isspace(c)) 
        {
            if (curr_state == SYMBOLS) {
                ++(*nw);
            }
            curr_state = SPACES;
        } 
        else
        {
            curr_state = SYMBOLS;
        }
        ++(*nc);             
    }
}
