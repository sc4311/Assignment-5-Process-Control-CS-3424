#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMANDS 6
#define MAX_ARGS 10

pid_t fork();

int main(int argc, char *argv[]) {
    char *commands[MAX_COMMANDS][MAX_ARGS];
    int command_count = 0;

    // Parse command line arguments
    int arg_count = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], ",") == 0) {
            commands[command_count][arg_count] = NULL;
            command_count++;
            arg_count = 0;
        } else {
            commands[command_count][arg_count] = argv[i];
            arg_count++;
        }
    }
    commands[command_count][arg_count] = NULL;
    command_count++; // Increment command_count after the last command

    // Execute commands
    for (int i = 0; i < command_count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            fprintf(stderr, "PID: %d, PPID: %d, CMD: (%s)\n", getpid(), getppid(), commands[i][0]);
            execvp(commands[i][0], commands[i]);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for child processes to finish
    for (int i = 0; i < command_count; i++) {
        wait(NULL);
    }

    return 0;
}