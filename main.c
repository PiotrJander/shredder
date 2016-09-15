#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

#define STDIN 0
#define STDOUT 1
#define STDERR 2


// TODO write Makefile
// TODO write README
// TODO check with valgrind
// TODO run on speclab

// TODO attribute outside sources
// TODO answer questions about signals in README


// ================
// Global variables
// ================

/*
 * Declared global so that it can be accessed by the alarm signal handler.
 */
static int pid;


// ========
// Typedefs
// ========

typedef void (*sighandler_t)(int);


// =====================
// Function declarations
// =====================

int atoi(const char* str);
int shredder(int time_limit);
size_t read_discard_overflow(char *buf, size_t nbyte);
void handle_fork(char *input, size_t input_length, int time_limit);
void parent(int time_limit);
void alarm_handler(int signal);
void child(char *input, size_t input_length);
void get_path(char *path, char *input, size_t input_length);


// System calls wrappers

void write_wrapper(int fildes, const void *buf, size_t nbyte);
size_t read_wrapper(int fildes, void *buf, size_t nbyte);
int fork_wrapper();
void wait_wrapper();
void execve_wrapper(const char *path, char *const argv[]);
void signal_wrapper(int signum, sighandler_t handler);
void kill_wrapper(int pid, int sig);


// ====================
// Function definitions
// ====================

int main(int argc, char** argv) {

    int time_limit;

    if (argc == 1) {
        time_limit = 0;
    } else {
        time_limit = atoi(argv[1]);
        if (time_limit == 0) {
            write(STDERR, "Invalid timeout argument.\n", 26);
            _exit(1);
        }
    }

    return shredder(time_limit);
}


/*
 * Parses a string into a base 10 integer.
 *
 * Like stdlib's atoi, returns 0 for invalid inputs.
 */
int atoi(const char* str) {
    // get the index of the last digit
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }

    if (i == 0) {
        // empty string
        return 0;
    }

    // now we want to be looping back
    int ret = 0;
    int base = 1;
    for (int j = i - 1; j >= 0 ; j--) {
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


int shredder(int time_limit) {

    char input[BUFFER_SIZE];

    while (1) {

        write_wrapper(STDOUT, "shredder# ", 10);

        size_t input_length = read_discard_overflow(input, BUFFER_SIZE);

        if (input_length == 0) {
            write_wrapper(STDERR, "Input longer than 1024 bytes.\n", 30);
            continue;
        }

        handle_fork(input, input_length, time_limit);

    }
}


/*
 *
 */
size_t read_discard_overflow(char *buf, size_t nbyte) {
    size_t input_length = read_wrapper(STDIN, buf, nbyte);

    if (input_length < nbyte || buf[nbyte - 1] == '\n') {
        return input_length;
    } else {
        while(read_wrapper(STDIN, buf, nbyte) == input_length);
        return 0;
    }
}


/*
 * Controls the flow of execution depending on whether we are in the child
 * or parent process.
 */
void handle_fork(char *input, size_t input_length, int time_limit) {

    // sets the global variable
    pid = fork_wrapper();

    if (pid == 0) {
        child(input, input_length);
    } else {
        parent(time_limit);
    }
}


void parent(int time_limit) {

    signal_wrapper(SIGALRM, alarm_handler);

    // apparently there are no exception to be handled with 'alarm'
    alarm((unsigned int) time_limit);

    wait_wrapper();

    // since the child process has already terminated, we can cancel the alarm, if any
    alarm(0);
}


void alarm_handler(int signal) {
    if (signal == SIGALRM) {
        kill_wrapper(pid, SIGKILL);
        write_wrapper(STDOUT, "Bwahaha ... tonight I dine on turtle soup\n", 42);
    }
}


/*
 * This function is run in the child process.
 *
 * If path to the command is invalid, exit with error code.
 *
 * Otherwise, execute the command.
 */
void child(char *input, size_t input_length) {
    char path[input_length];
    get_path(path, input, input_length);

    // we hardcoded the first element of argv to be "foo"; it doesn't seem to matter anyway
    char* argv[] = {"foo", NULL};

    execve_wrapper(path, argv);
}



/*
 * Copies the path-to-command part of the input to 'path'.
 *
 * Path-to-command part is assumed to be the part of the input until the first space.
 * Other whitespace is not handled.
 *
 * RETURN VALUE
 *
 * -1 if the input has length 0 or begins with a space, 0 otherwise.
 */
void get_path(char *path, char *input, size_t input_length) {
    // first find number of characters until the first space
    size_t path_size = input_length;
    for (size_t i = 0; i < input_length; ++i) {
        if (input[i] == ' ' || input[i] == '\n') {
            // at this point i equals the path size minus 1
            path_size = i;
            break;
        }
    }

    if (path_size == 0) {
        write(STDERR, "Input was empty or started with a space.\n", 41);
        _exit(1);
    }

    for (int j = 0; j < path_size; ++j) {
        path[j] = input[j];
    }
    path[path_size] = '\0';
}


// ====================
// System call wrappers
// ====================


void write_wrapper(int fildes, const void *buf, size_t nbyte) {
    if (write(fildes, buf, nbyte) == -1) {
        perror("write");
        _exit(1);
    }
}


size_t read_wrapper(int fildes, void *buf, size_t nbyte) {
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


int fork_wrapper() {
    int pid = fork();
    if (pid == -1) {
        perror("fork");
        _exit(1);
    } else {
        return pid;
    }
}


void wait_wrapper() {
    if (wait(NULL) == -1) {
        perror("wait");
        _exit(1);
    }
}

void execve_wrapper(const char *path, char *const *argv) {
    char* envp[] = {NULL};
    if (execve(path, argv, envp) == -1) {
        perror("execve");
        _exit(1);
    }
}

void signal_wrapper(int signum, sighandler_t handler) {
    if (signal(signum, handler) == SIG_ERR) {
        perror("signal");
        _exit(1);
    }
}

void kill_wrapper(int pid, int sig) {
    if (kill(pid, sig) == -1) {
        perror("kill");
        _exit(1);
    }
}
