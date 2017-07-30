#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

/* maximum number of characters for word to search */
#define WORD_MAX 256

/* maximum number of characters in line */
#define LINE_MAX 2048

/* delimiter for dictionary */
#define DELIMS "\t"

/* read dictionary file into trie structure */
unsigned int load_dictionary(const char* filename);

/* main function */
int main(int argc, char* argv[]) 
{   
	char word[WORD_MAX], *translation;
	int len;

	if(argc <= 1)
	{
		return 0;
    }
   
	init_trie();
	load_dictionary(argv[1]);

	do {
		printf("Enter word to translate: ");
		fflush(stdout);
		if(!fgets(word, WORD_MAX, stdin))
		{
			abort();
		}

		len = strlen(word);
		if(len > 0 && word[len - 1] == '\n') 
		{
			word[len - 1] = '\0';
			--len;
		}

		if(len == 0)
		{
			break;
		}
		
		translation = lookup_word(word);

		if(translation)
		{
			printf("%s -> %s\n", word, translation);
		}
		else
		{
			printf("\"%s\" not found\n", word);
		}
	} while(1);
	
	delete_trie();
	return 0;
}

unsigned int load_dictionary(const char* filename) 
{
	FILE* pfile;
	char line[LINE_MAX], *word, *translation;
	unsigned int icount = 0;

	if(!(pfile = fopen(filename, "r")))
	{
		return icount;
    }
    
	while(fgets(line, LINE_MAX, pfile)) {
		int len = strlen(line);
		/* handles only cases when line ends with \r\n */
		if(len > 0 && line[len - 2] == '\r' && line[len - 1] == '\n') {
			line[len - 2] = '\0';
			--len;
		}

		if(len == 0 || line[0] == '#')
		{
			continue;
        }
        
		word = line + strspn(line, DELIMS);
		if(!word[0])
		{
			continue;
		}
		translation = word + strcspn(word, DELIMS);
		*translation++ = '\0';
		translation += strspn(translation, DELIMS);

		if(add_word(word, translation))
		{
			icount++;
		}
	}

	fclose(pfile);
	return icount;
}

