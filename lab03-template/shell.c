#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

#define BUFLEN 1024

int main() {
    char buffer[BUFLEN];
    char* parsedinput;
    char* args[11];

    printf("Welcome to the Group20 shell! Enter commands, enter 'quit' to exit\n");

    do {
        // Print the terminal prompt and get input
        printf("$ ");
        char *input = fgets(buffer, sizeof(buffer), stdin);
        if (!input) {
            fprintf(stderr, "Error reading input\n");
            return -1;
        }

        // Clean and parse the input string
        parsedinput = (char*) malloc(BUFLEN * sizeof(char));
        size_t parselength = trimstring(parsedinput, input, BUFLEN);
        size_t argCount = tokenize(args, parsedinput, BUFLEN);
        args[argCount] = NULL;

        // Sample shell logic implementation
        if (strcmp(parsedinput, "quit") == 0) {
            printf("Bye!!\n");
            return 0;
        } else {
            // Check if the command contains a '/' character
            if (strchr(args[0], '/') != NULL) {
                // Execute as an absolute or relative path
                if (execve(args[0], args, NULL) == -1) {
                    fprintf(stderr, "Error running command in execve\n");
                    return -100;
                }
            } else {
                // Get the PATH environment variable
                char* path = getenv("PATH");
                if (path == NULL) {
                    fprintf(stderr, "PATH environment variable not set\n");
                    return -1;
                }

                // Search for the command in the PATH
                char* full_path = search_command_in_path(args[0], path);

                if (full_path != NULL) {
                    // Execute the command using the full path
                    if (execve(full_path, args, NULL) == -1) {
                        fprintf(stderr, "Error running command in execve\n");
                        return -100;
                    }
                    free(full_path);
                } else {
                    fprintf(stderr, "Command not found: %s\n", args[0]);
                }
            }
        }

        // Remember to free any memory you allocate!
        free(parsedinput);
    } while (1);

    return 0;
}