#include <stddef.h>
#include <stdlib.h>


typedef int bool;

#define true 1
#define false 0


/*
 * Find the length of member. The member ends with the first space.
 */
size_t find_member_length(char* member_start) {
    int i = 1;
//    while (*(member_start + i) != ' ' && *(member_start + i) != '\n' && *(member_start + i) != '\0') {
    while (*(member_start + i) != ' ' && *(member_start + i) != '\n') {
        i++;
    }
    return (size_t) i;
}


/*
 * Like in the standard library.
 */
char* strncpy_(char * dst, const char * src, size_t len) {
    bool null_char_encountered = false;
    for (int i = 0; i < len; ++i) {
        if (src[i] == '\0')  null_char_encountered = true;

        if (null_char_encountered) {
            dst[i] = '\0';
        } else {
            dst[i] = src[i];
        }
    }
    return dst;
}


/*
 * Number of space separated entities in the input. We count non-space characters preceded by a space.
 */
int get_input_member_number(char *input, size_t input_length) {

    int counter;
    if (input[0] == ' ' || input[0] == '\n') {
        counter = 0;
    } else {
        counter = 1;
    }

    for (int i = 1; i < input_length; ++i) {
        if (input[i] != ' ' && input[i - 1] == ' ')  counter++;
    }
    return counter;
}


/*
 * Copies the member from the input to the arguments vector.
 */
void copy_member(const char *remaining_input, char *input_member, size_t len) {
    strncpy_(input_member, remaining_input, len);
    input_member[len] = '\0';
}


/*
 * We assume the input ends with a newline and must have at least one character.
 *
 * If it has exactly one character, then this character is newline.
 */
char** parse_input(char *input, size_t input_length) {

    int input_member_number = get_input_member_number(input, input_length);

    char** input_members = malloc((input_member_number + 1) * sizeof(char*));
    input_members[input_member_number] = NULL;

    int member_i = 0;

    if (input[0] != ' ' && input[0] != '\n') {
        size_t len = find_member_length(input + 0);
        input_members[member_i] = malloc((len + 1) * sizeof(char));
        copy_member(input + 0, input_members[member_i], len);
        member_i++;
    }

    for (int i = 1; i < input_length; ++i) {
        if (input[i] != ' ' && input[i] != '\n' && input[i - 1] == ' ') {
            size_t len = find_member_length(input + i);
            input_members[member_i] = malloc((len + 1) * sizeof(char));
            copy_member(input + i, input_members[member_i], len);
            member_i++;
        }
    }

    return input_members;
}
