#ifndef H_HUFFMAN
#define H_HUFFMAN

/* to be called at start to initialize resources */
void init_huffman();

/* outputs compressed file fin into file fout */
void encode(FILE* fin, FILE* fout);

/* outputs original version of file fin into file fout */
void decode(FILE* fin, FILE* fout);

/* outputs symbol codes used in compression */
void dump_code(FILE* fp);

/* gets compression ratio for current file */
double compression_ratio();

/* to be called at the end to free resources */
void free_huffman();

#endif
