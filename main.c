#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

int main(int argc, char *argv[]){
	hashmap_t hashtable[HASH_SIZE];
	load_dictionary(argv[2], hashtable);
	char *misspelled[MAX_MISSPELLED];
	FILE *fp = fopen(argv[1], "r");
	char *line = NULL;
	size_t len = 0;
    	ssize_t read;
	while ((read = getline(&line, &len, fp)) != -1) {
        	printf("Retrieved line of length %lu :\n", read);
		printf("%s", line);
	}
	free(line);
    	fclose(fp);
	FILE *fp2 = fopen(argv[2], "r");
	int num_misspelled = check_words(fp2, hashtable, misspelled);
	printf("num misspelled is %d\n",num_misspelled);
	return 0;
}
