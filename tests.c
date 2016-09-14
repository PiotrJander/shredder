#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "main.c"

int tests_run = 0;


static char * test_get_path() {
    char* input = "world ";
    int input_length = 7;
    char path[8];
    get_path(path, input, input_length);
    printf("%s\n", path);
    mu_assert("test_get_path", strcmp(path, "world") == 0);
    return 0;
}

static char * test_get_path_invalid() {
    char* input = "";
    int input_length = 0;
    char path[8];
    int status = get_path(path, input, input_length);
    mu_assert("test_get_path_invalid", status == -1);
    return 0;
}

static char * test_execute_command() {
    char* input = "";
    int input_length = 0;
    child(input, input_length);
    mu_assert("test_execute_command", 0);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_get_path);
    mu_run_test(test_get_path_invalid);
    mu_run_test(test_execute_command);
    return 0;
}

//int main() {
//    char *result = all_tests();
//    if (result != 0) {
//        printf("%s\n", result);
//    }
//    else {
//        printf("ALL TESTS PASSED\n");
//    }
//    printf("Tests run: %d\n", tests_run);
//
//    return result != 0;
//}
