#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//Command to trim whitespace and ASCII control characters from buffer
//[Input] char* inputbuffer - input string to trim
//[Input] size_t bufferlen - size of input and output string buffers
//[Output] char* outputbuffer - output string after trimming 
//[Return] size_t - size of output string after trimming
char* search_command_in_path(const char* command, const char* path) {
    char* token = strtok((char*)path, ":");
    while (token != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", token, command);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path); // Return a dynamically allocated copy of the full path
        }
        token = strtok(NULL, ":");
    }
    return NULL; // Command not found in any directory in PATH
}

size_t trimstring(char* outputbuffer, const char* inputbuffer, size_t bufferlen)
{   
    memcpy(outputbuffer, inputbuffer, bufferlen*sizeof(char));

    for(size_t ii = strlen(outputbuffer)-1; ii >=0; ii--){
        if(outputbuffer[ii] < '!') //In ASCII '!' is the first printable (non-control) character
        {
            outputbuffer[ii] = 0;
        }else{
            break;
        }    
    }

    return strlen(outputbuffer);
}

size_t tokenize(char* args[], char *input, size_t buflen) {
    size_t count = 0;
    char* token = strtok(input, " ");
    while (token != NULL && count < buflen) {
        args[count++] = token;
        token = strtok(NULL, " ");
    }
    return count;
}

//Command to trim the input command to just be the first word
//[Input] char* inputbuffer - input string to trim
//[Input] size_t bufferlen - size of input and output string buffers
//[Output] char* outputbuffer - output string after trimming 
//[Return] size_t - size of output string after trimming
size_t firstword(char* outputbuffer, const char* inputbuffer, size_t bufferlen)
{
    // Check for invalid inputs
    if (outputbuffer == NULL || inputbuffer == NULL || bufferlen == 0) {
        return 0;
    }

    // Initialize variables
    size_t input_len = strlen(inputbuffer);
    size_t output_len = 0;

    // Iterate through the input string until a space or null character is found
    for (size_t i = 0; i < input_len && i < bufferlen - 1; i++) {
        if (inputbuffer[i] != ' ') {
            outputbuffer[output_len] = inputbuffer[i];
            output_len++;
        } else {
            break;  // Stop when a space is encountered
        }
    }

    // Null-terminate the output string
    outputbuffer[output_len] = '\0';

    return output_len;
}

//Command to test that string only contains valid ascii characters (non-control and not extended)
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] bool - true if no invalid ASCII characters present
bool isvalidascii(const char* str,size_t BUFLEN) {
    while (*str) {
        if (*str < 0 || *str > 127) {
            return false; // Found a non-ASCII character
        }
        str++;
    }
    return true; // All characters are valid ASCII
}

//Command to find location of pipe character in input string
//[Input] char* inputbuffåer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] int - location in the string of the pipe character, or -1 pipe character not found

// int findpipe(const char* inputbuffer, size_t bufferlen) {
 
//     return -1; // Pipe character not found
// }

  



int findpipe(const char* inputbuffer, size_t bufferlen) {
    for (size_t i = 0; i < bufferlen; i++) {
        if (inputbuffer[i] == '|') {
            return i; // Return the position of the pipe character
        }
    }
    return -1; // Pipe character not found
}

//Command to test whether the input string ends with "&" and
//thus represents a command that should be run in background
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] bool - true if string ends with "&"

bool runinbackground(const char* inputbuffer, size_t bufferlen) {
    // Ensure that bufferlen is not greater than the actual string length
    int stringlen = strlen(inputbuffer);
    if (bufferlen > stringlen) {
        bufferlen = stringlen;
    }

   

    // Check if the last character of the string is '&'

    if (bufferlen > 0 && inputbuffer[bufferlen - 2] == '&') {
        
        return true; // The string ends with '&', indicating a background command
    }

    return false; // The string does not end with '&'
}
char* searchCommand(char* command, char* path) {
    const char* delimiter = ":";
    // Initialize strtok with the string and delimiter
    char *token = strtok(path, delimiter);
    // Loop through the tokens
    while (token != NULL) {
        // Concatenate a "/" to the end of the token
        size_t token_len = strlen(token);
        char* modified_token = (char*)malloc(token_len + 2); // +2 for the "/" and null terminator
        strcpy(modified_token, token);
        strcat(modified_token, "/");
        
        // Print or use the modified token
       

        // Free the dynamically allocated memory
        free(modified_token);

        // Get the next token
        token = strtok(NULL, delimiter);
    }
}
void checkCompletedBackgroundProcesses() {
  int status;
  pid_t pid;

  while ((pid = waitpid(-1, & status, WNOHANG)) > 0) {
    printf("Background command (PID: %d) terminated\n", pid);
  }
}


