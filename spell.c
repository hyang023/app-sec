#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

//function check_word(string word, hashmap hashtable[])
bool check_word(const char* word, hashmap_t hashtable[]) {  
    //string lower_word = to_lower(word)
    int a = strlen(word);
    char* lower_word = malloc(a);
    strcpy(lower_word, word);
    for (int i=0; i<strlen(lower_word); i++){
      lower_word[i]=tolower(lower_word[i]);
    }
    //int hash_val = hash_function(lower_word) otherwise as:
    //Set int bucket to the output of hash_function(word).
    int hash_val = hash_function(lower_word);
    //Set hashmap_t cursor equal to hashmap[bucket].
    hashmap_t cursor = hashtable[hash_val];
    //While cursor is not NULL:
    while (cursor != NULL){
        //If word equals cursor->word:
        if (word == cursor->word)
            return 1;
        //Set curosr to cursor->next.
        cursor = cursor->next;
    }
    return 0;
}

//function load_dictionary(string dictionary, hashmap hashtable[])
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    //Initialize all values in hash table to NULL.
    for (int i=0; i<HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    /*
    Open dict_file from path stored in dictionary.
    If dict_file is NULL:
        return false.
    While word in dict_file is not EOF (end of file):
        Set hashmap_t new_node to a new node.
        Set new_node->next to NULL.
        Set new_node->word equal to word.
        Set int bucket to hash_function(word).
        if hashtable[bucket] is NULL:
            Set hashtable[bucket] to new_node.
        else:
            Set new_node->next to hashtable[bucket].
            Set hashtable[bucket] to new_node.
    Close dict_file.
    */
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
 
    stream = fopen(dictionary_file, "r");
    if (stream == NULL)
        return 0;
 
    while ((read = getline(&line, &len, stream)) != -1) {
        //NEED TO MALLOC
        //printf("Retrieved line of length %u :\n", read);
        //printf("%s", line);
        //Set hashmap_t new_node to a new node.
        hashmap_t new_node = (struct node*) malloc(sizeof(struct node));;
        //Set new_node->next to NULL.
        new_node->next = NULL;
        //Set new_node->word equal to word.
        strcpy(new_node->word,line);
        //Set int bucket to hash_function(word).
        int bucket = hash_function(line);
        //if hashtable[bucket] is NULL:
        if (hashtable[bucket] == NULL){
           //Set hashtable[bucket] to new_node.
            hashtable[bucket] = new_node;
        } 
        //else:
        else{
            //Set new_node->next to hashtable[bucket].
            new_node->next = hashtable[bucket];
            //Set hashtable[bucket] to new_node.
            hashtable[bucket] = new_node;
        }
    }
 
    free(line);
    fclose(stream);
    return 1;
}

//function check_words(file fp, hashmap hashtable[], string misspelled[])
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    //Set int num_misspelled to 0.
    int num_misspelled = 0;

    char *line = NULL;
    //char *savePtr;
    char *token;
    size_t len = 0;
    ssize_t read;
 
    //argc[1];
    //fp = fopen("wordlist.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
 
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %lu :\n", read);
        //for(token = strtok_r(read, " ", &savePtr); token != NULL; strtok_r(NULL, " ", &savePtr))
        printf("check words line is: %s", line);
        //char* token; 
        char* rest = strdup(line); 
        //char *rest;
        //strtok(rest, " ");
        while ((token = strtok_r(rest, " !,.?;-:\"\'", &rest))){
            printf("token is: \"%s\"\n", token);
            //remove punctuation from beginning and end of word
            //if not check_word(word):
            if (!(check_word(token,hashtable))) {
                //append word to misspelled
                //NEED TO MALLOC
                //For a variable-length string, do:
                //char *string = malloc(length + 1);
                misspelled[num_misspelled]=token;
                //increment num_misspelled 
                num_misspelled++;
            }
        }
    }
 
    free(line);
    fclose(fp);
    /*While line in fp is not EOF (end of file):
        Read the line.
        Split the line on spaces.
        For each word in line:
            Remove punctuation from beginning and end of word.
            If not check_word(word):
                Append word to misspelled.
                Increment num_misspelled.
    */
        
    //Return num_misspelled.
    return num_misspelled;
}

//citation: https://stackoverflow.com/questions/2661766/how-do-i-lowercase-a-string-in-c
//citation: https://www.rosettacode.org/wiki/Read_a_file_line_by_line#C
