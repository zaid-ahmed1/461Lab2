#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Command to find location of pipe character in input string
int findpipe(const char* inputbuffer, size_t bufferlen) {
    for (size_t i = 0; i < bufferlen; i++) {
        if (inputbuffer[i] == '|') {
            return i; // Return the position of the pipe character
        }
    }
    return -1; // Pipe character not found
}

int main() {
    char input[1024];
    char* args1[64];
    char* args2[64];

    printf("Enter two commands separated by a pipe (|):\n");
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0'; // Remove the newline character

    int pipePos = findpipe(input, strlen(input));

    if (pipePos == -1) {
        fprintf(stderr, "No pipe character found in input\n");
        return 1;
    }

    // Split the input into two commands
    input[pipePos] = '\0';
    char* command1 = input;
    char* command2 = input + pipePos + 1;

    // Tokenize the commands
    char* token = strtok(command1, " ");
    int i = 0;
    while (token != NULL) {
        args1[i++] = token;
        token = strtok(NULL, " ");
    }
    args1[i] = NULL;

    token = strtok(command2, " ");
    i = 0;
    while (token != NULL) {
        args2[i++] = token;
        token = strtok(NULL, " ");
    }
    args2[i] = NULL;

    int pipe_fds[2];
    pid_t pid1, pid2;
    printf("%s" , command1);
    printf("%s" , command2);
    if (pipe(pipe_fds) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    pid1 = fork();

    if (pid1 == 0) { // First child process (producer)
        close(pipe_fds[0]); // Close the reading end of the pipe

        dup2(pipe_fds[1], STDOUT_FILENO); // Redirect stdout to the writing end of the pipe
        close(pipe_fds[1]);
          
        execvp(args1[0], args1); // Execute the first command
        perror("execvp command1");
        exit(1);
    } else if (pid1 < 0) {
        perror("Fork failed");
        return 1;
    }

    pid2 = fork();

    if (pid2 == 0) { // Second child process (consumer)
        close(pipe_fds[1]); // Close the writing end of the pipe

        dup2(pipe_fds[0], STDIN_FILENO); // Redirect stdin from the reading end of the pipe
        close(pipe_fds[0]);
  
        execvp(args2[0], args2); // Execute the second command
        perror("execvp command2");
        exit(1);
    } else if (pid2 < 0) {
        perror("Fork failed");
        return 1;
    }

    close(pipe_fds[0]); // Close both ends of the pipe in the parent
    close(pipe_fds[1]);

    // Wait for both child processes to complete
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
