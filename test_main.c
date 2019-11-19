#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    char* hash_word1 = "first";
    int hash_val1 = hash_function(hash_word1);
    char* hash_word2 = "second";
    int hash_val2 = hash_function(hash_word2);
    char* hash_word3 = "third";
    int hash_val3 = hash_function(hash_word3);
    char* hash_word4 = "test";
    int hash_val4 = hash_function(hash_word4);
    // Here we can test if certain words ended up in certain buckets
    // to ensure that our load_dictionary works as intended. I leave
    // this as an exercise.
    int words_in_correct_bucket = 0;
    hashmap_t cursor = hashtable[hash_val1];
    while (cursor != NULL){
        if (strcmp(hash_word1, cursor->word) == 0){
            word_in_correct_bucket += 1;
        }
        cursor = cursor->next;
    }
    ck_assert(words_in_correct_bucket == 1);
}
END_TEST

START_TEST(test_remove_punctuation_normal)
{
    const char* compare_word1 = "quotation";
    char* test_word1 = "quotation.";
    test_word1 = remove_punctuation(test_word1);
    char* test_word2 = ".quotation";
    test_word2 = remove_punctuation(test_word2);
    char* test_word3 = ".quotation.";
    test_word3 = remove_punctuation(test_word3);
    char* test_word4 = "quotation!!!!!";
    test_word4 = remove_punctuation(test_word4);
    char* test_word5 = "!!!!!quotation!!!!!";
    test_word5 = remove_punctuation(test_word5);
    ck_assert(strcmp(test_word1, compare_word1) == 0);
    ck_assert(strcmp(test_word2, compare_word1) == 0);
    ck_assert(strcmp(test_word3, compare_word1) == 0);
    ck_assert(strcmp(test_word4, compare_word1) == 0);
    ck_assert(strcmp(test_word5, compare_word1) == 0);
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* correct_word2 = "JUSTICE";
    const char* punctuation_word_2 = "pl.ace";
    const char* number_word = "12345";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(check_word(correct_word2, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    ck_assert(!check_word(number_word, hashtable));
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    //tcase_add_test(check_word_case, test_remove_punctuation_normal);
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

