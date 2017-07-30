#include <stdlib.h>
#include <stdio.h>

static char* p_nxt_tok = NULL;

char* strtok(char* text, const char* delims);
int strpos(const char* str, const char ch);
size_t strspn(const char* str, const char* delims);
size_t strcspn(const char* str, const char* delims);

int main() {
    char str[80] = "This is - www.tutorialspoint.com - website";
    const char s[4] = "- .";
    char *token;
   
    /* get the first token */
    token = strtok(str, s);
   
    /* walk through other tokens */
    while(token != NULL) {
        printf(" '%s'\n", token);
        token = strtok(NULL, s);
    }
    
    return 0;
}

char* strtok(char* text, const char* delims) {
    if(!text) {
        text = p_nxt_tok;
    }
    text += strspn(text, delims);
    if(*text == '\0') {
        return NULL;
    }
    p_nxt_tok = text + strcspn(text, delims);
    if(*p_nxt_tok != '\0') {
        *p_nxt_tok++ = '\0';
    }   
    return text;
}

int strpos(const char* str, const char ch) {
    int pos = 0;
    while(*str && *str != ch) {
        ++pos;
        ++str;
    }
    return *str != '\0' ? pos : -1;
}

size_t strspn(const char* str, const char* delims) {
    size_t pos = 0;
    while(*str && strpos(delims, *str++) >= 0) {
        ++pos;
    }
    return pos;
}

size_t strcspn(const char* str, const char* delims) {
    size_t pos = 0;
    while(*str && strpos(delims, *str++) < 0) {
        ++pos;
    }
    return pos;
}

