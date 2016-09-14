#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#define BUFFER_SIZE 1024

#define STDIN 0
#define STDOUT 1
#define STDERR 2


// TODO rewrite atoi
// TODO parse args
//


int shredder(int time_limit);
void handle_fork(char *input, size_t input_length, int time_limit);
void parent(int pid, int time_limit);
void child(char *input, size_t input_length);
int get_path(char *path, char *input, size_t input_length);

void write_wrapper(int fildes, const void *buf, size_t nbyte);
ssize_t read_wrapper(int fildes, void *buf, size_t nbyte);


int main(int argc, char** argv) {

    int time_limit;

    if (argc == 0) {
        time_limit = 0;
    } else {
        time_limit = atoi(argv[1]);
    }

    return shredder(time_limit);
}


int shredder(int time_limit) {

    char input[BUFFER_SIZE];

    while (1) {

        write_wrapper(STDOUT, "shredder# ", 10);

        ssize_t input_length = read_wrapper(STDIN, input, BUFFER_SIZE);

        handle_fork(input, input_length, time_limit);

    }
}


/*
 * Controls the flow of execution depending on whether we are in the child
 * or parent process.
 */
void handle_fork(char *input, ssize_t input_length, int time_limit) {
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork");
            _exit(1);
        case 0:
            child(input, input_length);
        default:
            parent(pid, time_limit);
    }
}


void parent(int pid, int time_limit) {

    void signal_handler(int signal) {
        if (signal == SIGALRM) {
            if (kill(pid, SIGKILL) == -1) {
                perror("kill");
                _exit(1);
            }
        }
    }

    if (signal(SIGALRM, signal_handler) == SIG_ERR) {
        perror("signal");
        _exit(1);
    }

    alarm(time_limit);

    int status = wait(NULL);
    if (status == -1) {
        perror("wait");
        _exit(1);
    }
}


/*
 * This function is run in the child process.
 *
 * If path to the command is invalid, exit with error code.
 *
 * Otherwise, execute the command.
 */
void child(char *input, ssize_t input_length) {
    char path[input_length + 1];  // + 1 for null char;
    int status = get_path(path, input, input_length);

    if (status == -1) {
        write(STDERR, "Input was empty or started with a space.\n", 41);
        _exit(1);
    }

    char* argv[] = {"ls", NULL};
    char* envp[] = {NULL};
    status = execve(path, argv, envp);
    if (status == -1) {
        perror("execve");
        _exit(1);
    }
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
int get_path(char *path, char *input, ssize_t input_length) {
    // first find number of characters until the first space
    ssize_t path_size = input_length;
    for (size_t i = 0; i < input_length; ++i) {
        if (input[i] == ' ' || input[i] == '\n') {
            // at this point i equals the path size minus 1
            path_size = i;
            break;
        }
    }

    if (path_size == 0) {
        return -1;
    }

    for (int j = 0; j < path_size; ++j) {
        path[j] = input[j];
    }
    path[path_size] = '\0';

    write(STDOUT, path, path_size);

    return 0;
}

void write_wrapper(int fildes, const void *buf, size_t nbyte) {
    if (write(fildes, buf, nbyte) == -1) {
        perror("write");
        _exit(1);
    }
}

ssize_t read_wrapper(int fildes, void *buf, size_t nbyte) {
    ssize_t status = read(fildes, buf, nbyte);

    switch (status) {
        case 0:
            _exit(0);
        case -1:
            perror("read");
            _exit(1);
        default:
            return status;
    }
}

// system call wrappers

