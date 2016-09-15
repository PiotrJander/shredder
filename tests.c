#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "main.c"

int tests_run = 0;


//static char * test_get_path() {
//    char* input = "hello ";
//    size_t input_length = 7;
//    char path[7];
//    get_path(path, input, input_length);
//    printf("%s\n", path);
//    mu_assert("test_get_path", strcmp(path, "hello") == 0);
//    return 0;
//}
//
//static char * test_get_path_empty() {
//    char* input = "";
//    size_t input_length = 0;
//    char path[1];
//    get_path(path, input, input_length);
//    mu_assert("test_get_path_invalid", strcmp(path, "") == 0);
//    return 0;
//}

//static char * test_execute_command() {
//    char* input = "";
//    int input_length = 0;
//    child(input, input_length);
//    mu_assert("test_execute_command", 0);
//    return 0;
//}

static char * test_parse_123() {
    mu_assert("parse 123", atoi("123") == 123);
    return 0;
}

static char * test_parse_empty_string() {
    mu_assert("parse empty", atoi("") == 0);
    return 0;
}

static char * test_parse_invalid_string() {
    mu_assert("parse invalid", atoi("12a3") == 0);
    return 0;
}

static char * all_tests() {
//    mu_run_test(test_get_path);
//    mu_run_test(test_get_path_empty);
//    mu_run_test(test_execute_command);
    mu_run_test(test_parse_123);
    mu_run_test(test_parse_empty_string);
    mu_run_test(test_parse_invalid_string);
    return 0;
}

int main() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
