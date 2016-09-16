#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include "parse.h"

#define BUFFER_SIZE 1024

#define STDIN 0
#define STDOUT 1
#define STDERR 2


// ================
// Global variables
// ================

/*
 * Child process ID. Declared global so that it can be accessed by the alarm signal handler.
 */
static int pid;


// ========
// Typedefs
// ========

typedef void (*sighandler_t)(int);


// =====================
// Function declarations
// =====================

int
atoi_(const char *str);

int
shredder(int time_limit);

size_t
read_discard_overflow(char *buf, size_t nbyte);

void
handle_fork(char **args, int time_limit);

void
parent(int time_limit);

void
alarm_handler(int signal);

void
sigint_handler(int signal);

void
child(char **args);


// System calls wrappers

void
write_wrapper(int fildes, const void *buf, size_t nbyte);

size_t
read_wrapper(int fildes, void *buf, size_t nbyte);

int
fork_wrapper();

void
wait_wrapper();

void
execve_wrapper(const char *path, char *const argv[]);

void
signal_wrapper(int signum, sighandler_t handler);

void
kill_wrapper(int pid, int sig);


// ====================
// Function definitions
// ====================

int main(int argc, char **argv)
{
    int time_limit;

    if (argc == 1) {
        time_limit = 0;
    } else {
        time_limit = atoi_(argv[1]);
        if (time_limit == 0) {
            write(STDERR, "Invalid timeout argument.\n", 26);
            _exit(1);
        }
    }

    return shredder(time_limit);
}


int shredder(int time_limit)
{
    char input[BUFFER_SIZE];

    while (1) {

        write_wrapper(STDOUT, "shredder# ", 10);

        size_t input_length = read_discard_overflow(input, BUFFER_SIZE);

        if (input_length == 0) {
            write_wrapper(STDERR, "Input longer than 1024 bytes.\n", 30);
            continue;
        }

        char **args = parse_input(input, input_length);

        if (args[0] == NULL) {
            free(args);
            continue;
        } else {
            handle_fork(args, time_limit);

            // free memory
            int i = 0;
            while (args[i] != NULL) {
                free(args[i]);
                i++;
            }
            free(args);
        }
    }
}


/*
 * Reads from STDIN, returns the input size.
 *
 * If input is longer than 1024 bytes, returns 0. Rest of the input is read until empty.
 */
size_t read_discard_overflow(char *buf, size_t nbyte)
{
    size_t input_length = read_wrapper(STDIN, buf, nbyte);

    if (input_length < nbyte || buf[nbyte - 1] == '\n') {
        return input_length;
    } else {
        while (read_wrapper(STDIN, buf, nbyte) == nbyte);
        return 0;
    }
}


/*
 * Controls the flow of execution depending on whether we are in the child
 * or parent process.
 */
void handle_fork(char **args, int time_limit)
{
    // pid is a global variable
    pid = fork_wrapper();

    if (pid == 0) {
        child(args);
    } else {
        parent(time_limit);
    }
}


void parent(int time_limit)
{
    signal_wrapper(SIGALRM, alarm_handler);
    signal_wrapper(SIGINT, sigint_handler);

    // apparently there are no exception to be handled with 'alarm'
    alarm((unsigned int) time_limit);

    wait_wrapper();

    // since the child process has already terminated, we can cancel the alarm, if any
    alarm(0);

    // also restore default SIGINT behaviour
    signal_wrapper(SIGINT, NULL);
}


void alarm_handler(int signal)
{
    if (signal == SIGALRM) {
        kill_wrapper(pid, SIGKILL);
        write_wrapper(STDOUT, "Bwahaha ... tonight I dine on turtle soup\n", 42);
    }
}


void sigint_handler(int signal)
{
    if (signal == SIGINT) {
        kill_wrapper(pid, SIGKILL);
    }
}


/*
 * This function is run in the child process.
 *
 * If path to the command is invalid, exit with error code.
 *
 * Otherwise, execute the command.
 *
 * NOTE ABOUT MEMORY LEAKS: we do not free 'args' here. If execve succeeds, memory gets replaced anyway.
 * If it fails, the child process terminates and its whole memory is freed by the OS.
 */
void child(char **args)
{
    execve_wrapper(args[0], args);
}


// ====================
// Helper functions
// ====================

/*
 * Parses a string into a base 10 integer.
 *
 * Like stdlib's atoi, returns 0 for invalid inputs.
 */
int atoi_(const char *str)
{
    // get the index of the last digit
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }

    // empty string
    if (i == 0) {
        return 0;
    }

    // now loop back
    int ret = 0;
    int base = 1;
    for (int j = i - 1; j >= 0; j--) {
        int digit = str[j] - '0';
        if (digit < 0 || digit > 9) {
            // non-digit character
            return 0;
        }
        ret += digit * base;
        base *= 10;
    }
    return ret;
}


// ====================
// System call wrappers
// ====================


void write_wrapper(int fildes, const void *buf, size_t nbyte)
{
    if (write(fildes, buf, nbyte) == -1) {
        perror("write");
        _exit(1);
    }
}


size_t read_wrapper(int fildes, void *buf, size_t nbyte)
{
    ssize_t status = read(fildes, buf, nbyte);

    switch (status) {
        case 0:
            _exit(0);
        case -1:
            perror("read");
            _exit(1);
        default:
            return (size_t) status;
    }
}


int fork_wrapper()
{
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        _exit(1);
    } else {
        return pid;
    }
}


void wait_wrapper()
{
    if (wait(NULL) == -1) {
        perror("wait");
        _exit(1);
    }
}

void execve_wrapper(const char *path, char *const *argv)
{
    char *envp[] = {NULL};
    if (execve(path, argv, envp) == -1) {
        perror("execve");
        _exit(1);
    }
}

void signal_wrapper(int signum, sighandler_t handler)
{
    if (signal(signum, handler) == SIG_ERR) {
        perror("signal");
        _exit(1);
    }
}

void kill_wrapper(int pid, int sig)
{
    if (kill(pid, sig) == -1) {
        perror("kill");
        _exit(1);
    }
}
