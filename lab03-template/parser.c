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
    //TO DO: Implement this function
    return 0;
}

//Command to test that string only contains valid ascii characters (non-control and not extended)
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] bool - true if no invalid ASCII characters present
bool isvalidascii(const char* inputbuffer, size_t bufferlen)
{
    //TO DO: Correct this function so that the second test string fails
    size_t testlen = bufferlen;
    size_t stringlength = strlen(inputbuffer);
    if(strlen(inputbuffer) < bufferlen){
        testlen = stringlength;
    }

    bool isValid = true;
    for(size_t ii = 0; ii < testlen; ii++)
    {
        isValid &= ((unsigned char) inputbuffer[ii] <= '~'); //In (lower) ASCII '~' is the last printable character
    }

    return isValid;
}

//Command to find location of pipe character in input string
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] int - location in the string of the pipe character, or -1 pipe character not found
int findpipe(const char* inputbuffer, size_t bufferlen){
    //TO DO: Implement this function

    return -1;
}

//Command to test whether the input string ends with "&" and
//thus represents a command that should be run in background
//[Input] char* inputbuffer - input string to test
//[Input] size_t bufferlen - size of input buffer
//[Return] bool - true if string ends with "&"
bool runinbackground(const char* inputbuffer, size_t bufferlen){
    //TO DO: Implement this function

    return false;
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
        printf("%s\n", modified_token);

        // Free the dynamically allocated memory
        free(modified_token);

        // Get the next token
        token = strtok(NULL, delimiter);
    }
}

