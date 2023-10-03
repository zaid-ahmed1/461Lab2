#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

#define BUFLEN 1024

//To Do: This base file has been provided to help you start the lab, you'll need to heavily modify it to implement all of the features

int main() {
    char buffer[1024];
    char* parsedinput;
    char* args[11];
    char newline;
    char* path = "/usr/bin/";

    printf("Welcome to the Group20 shell! Enter commands, enter 'quit' to exit\n");
    do {
        //Print the terminal prompt and get input
        printf("$ ");
        char *input = fgets(buffer, sizeof(buffer), stdin);
        if(!input)
        {
            fprintf(stderr, "Error reading input\n");
            return -1;
        }
        
        //Clean and parse the input string
        parsedinput = (char*) malloc(BUFLEN * sizeof(char));
        size_t parselength = trimstring(parsedinput, input, BUFLEN);
        size_t argCount = tokenize(args, parsedinput, BUFLEN);
        printf("%lu\n", argCount);
        args[argCount] = NULL;
                //Sample shell logic implementation
        if ( strcmp(parsedinput, "quit") == 0 ) {
            printf("Bye!!\n");
            return 0;
        }
        else {
            pid_t forkV = fork();
            if ( forkV == 0 ) {

                char* firstArg = (char*)malloc(strlen(path) + strlen(args[0]) + 1); // New temp variable
                strcpy(firstArg, path);                                             // 
                // Concatenate the second string to the result
                strcat(firstArg, args[0]);
                args[0] = firstArg;

                if(execve(args[0], args, NULL) == -1)
                {
                    fprintf(stderr, "Error running command in execve\n");
                    return -100;
                }

            } 
            else {
                wait(NULL);
            }
        }

        //Remember to free any memory you allocate!
        free(parsedinput);
    } while ( 1 );

    return 0;
}
