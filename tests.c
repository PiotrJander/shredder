#include <stdio.h>
#include <string.h>

#include "minunit.h"

#include "main.c"
#include "parse.c"


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

static char * test_atoi() {
    mu_assert("atoi 1", atoi_("123") == 123);
    return 0;
}

static char * test_atoi2() {
    mu_assert("atoi 2", atoi_("") == 0);
    return 0;
}

static char * test_atoi3() {
    mu_assert("atoi 3", atoi_("12a3") == 0);
    return 0;
}

static char * test_get_input_member_number() {
    char str[] = "asd fgh jkl";
    mu_assert("get_input_member_number", get_input_member_number(str, 11) == 3);
    char str2[] = " asd fgh jkl  ";
    mu_assert("get_input_member_number 2", get_input_member_number(str2, 14) == 3);
    return 0;
}

static char * test_strncpy() {
    char src[] = "asd fgh jkl";
    char dst[12];
    char* copied = strncpy_(dst, src, 12);
    mu_assert("strncpy", strcmp(src, dst) == 0);
    mu_assert("strncpy", dst == copied);
    return 0;
}

static char * test_find_member_length() {
    char str[] = "asd asd";
    mu_assert("find_member_length", find_member_length(str) == 3);
    return 0;
}

static char * test_parse_input() {
    char str[] = "abc def ghj";
    char** argv = parse_input(str, 12);
    mu_assert("parse input", strcmp(argv[0], "abc") == 0);
    mu_assert("parse input", strcmp(argv[1], "def") == 0);
    mu_assert("parse input", strcmp(argv[2], "ghj") == 0);
    mu_assert("parse input", argv[3] == NULL);
    return 0;
}

static char * test_parse_input2() {
    char str[] = " abc def ghj ";
    char** argv = parse_input(str, 12);
    mu_assert("parse input", strcmp(argv[0], "abc") == 0);
    mu_assert("parse input", strcmp(argv[1], "def") == 0);
    mu_assert("parse input", strcmp(argv[2], "ghj") == 0);
    mu_assert("parse input", argv[3] == NULL);
    return 0;
}

static char * all_tests() {
//    mu_run_test(test_get_path);
//    mu_run_test(test_get_path_empty);
//    mu_run_test(test_execute_command);
//    mu_run_test(test_parse_123);
//    mu_run_test(test_parse_empty_string);
//    mu_run_test(test_parse_invalid_string);

    mu_run_test(test_atoi);
    mu_run_test(test_atoi2);
    mu_run_test(test_atoi2);
    mu_run_test(test_get_input_member_number);
    mu_run_test(test_strncpy);
    mu_run_test(test_find_member_length);
    mu_run_test(test_parse_input);
    mu_run_test(test_parse_input2);
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
