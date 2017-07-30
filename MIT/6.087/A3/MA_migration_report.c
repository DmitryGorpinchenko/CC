#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_CODE_SIZE 5
#define MAX_BUFFER_SIZE 256
#define STATE_NAME_ID 5
#define FIELDS_NUMBER 9

const char* MA = "\"25\"";

int main(int argc, char* argv[]) {
	FILE* fp = argc > 1 ? fopen(argv[1], "r") : NULL;
	if (fp == NULL) {
	    exit(1);
	}
	size_t i;
	uint64_t people_count, total_count = 0;
	char buffer[MAX_BUFFER_SIZE]; 
	fgets(buffer, MAX_BUFFER_SIZE, fp); //read headers
	while (fscanf(fp, "%s", buffer) != EOF) {
		if (!strcmp(buffer, MA)) {
			for (i = 1; i <= FIELDS_NUMBER - 2; ++i) {
				fscanf(fp, "%s", buffer);
				if (i == STATE_NAME_ID) {
					printf("%-25s", buffer);
				}
			}
			fscanf(fp, "%lu", &people_count);
			total_count += people_count;
			printf("%12lu\n", people_count);
		} else {
            fgets(buffer, MAX_BUFFER_SIZE, fp);  
		}
	}
	printf("\n%-25s%12lu\n", "TOTAL", total_count);
	fclose(fp);
	return 0;
}
