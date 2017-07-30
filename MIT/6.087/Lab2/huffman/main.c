#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

int main()
{
    FILE* book = fopen(   "book.txt", "r");
    FILE* enc  = fopen("encoded.txt", "w");
    FILE* dec  = fopen("decoded.txt", "w");
    
    init_huffman();
    
    encode(book, enc);
    fclose(enc);
    fclose(book);
    
    enc = fopen("encoded.txt", "r");
    decode(enc, dec);
    
    printf("**************** symbol codes ********************\n");
    dump_code(stdout);
    printf("\ncompression ratio: %f\n", compression_ratio());
    fclose(enc);
    fclose(dec);
    free_huffman();
    return 0;
}
