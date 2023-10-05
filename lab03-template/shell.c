#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

#define BUFLEN 1024
void handleChildCompletion(pid_t pid);

void handleChildCompletion(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    printf("Background process (PID: %d) has completed\n", pid);
}

int main() {
    char buffer[BUFLEN];
    char* parsedinput;
    char* args[11];
    int status;
    pid_t terminated_pid = 3;
    pid_t id_to_check = 1;
          


    printf("Welcome to the Group20 shell! Enter commands, enter 'quit' to exit\n");

do {
    // Print the terminal prompt and get input
     pid_t id_to_check =  waitpid(-1, &status , WNOHANG);
    //  if(id_to_check == terminated_pid && id_to_check != 0){
    //     printf("Background process %d  %d terminated ", terminated_pid, id_to_check);
    //     id_to_check = 1;
    //     terminated_pid = 0;
    //     continue;
    //  }
  


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
    int pipePos = findpipe(input, strlen(input));
    bool check = runinbackground(input,strlen(input));  
    if(check){
        
          args[argCount - 1] = NULL;
    }
  
      
    
    // Sample shell logic implementation
    if (strcmp(parsedinput, "quit") == 0) {
        printf("Bye!!\n");
        break;  // Use break to exit the loop
    } else {
      
         if(pipePos != -1){
            //  char input[1024];
            char* args1[64];
            char* args2[64];

    // printf("Enter two commands separated by a pipe (|):\n");
    // fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0'; // Remove the newline character

    // int pipePos = findpipe(input, strlen(input));

    // if (pipePos == -1) {
    //     fprintf(stderr, "No pipe character found in input\n");
    //     return 1;
    // }

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

          
    

        }
        else{
        pid_t forkV = fork();
        
        // Check if the command contains a '/' character
        if (forkV == 0) { //child process
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
                    return -100;
                }
        }
        
        }
        else {
            {
            if(!check){
     
             wait(NULL);
    
            }
            else{
               // Check if any child process has terminated without waiting
                
    
                 

       

            }

            } 
          
        }
        }

    }



  
} while (1);

    return 0;
}






