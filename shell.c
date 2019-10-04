#include "shell.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

void executeCommand(char* args[], int numTokens)
{
    if(numTokens > 0)
    {
        if(internalCmd(args, numTokens) == 0) externalCmd(args, numTokens);
    }
}

int internalCmd(char* args[], int numTokens)
{
    int echo = strcmp(args[0], "echo") == 0;
    int end = strcmp(args[0], "end") == 0;

    if( (echo || end) && strcmp(args[numTokens-1], "&") == 0)
    {
        printf("Error: Invalid use of '&'\n");
        return -1;
    }

    if(echo)
    {
        for(int i = 1; i < numTokens; i++) printf("%s ", args[i]);
        printf("\n");
        return 1;
    }
    else if(end)
    {
        running = 0;
        return 1;
    }

    return 0;
}

int externalCmd(char* args[], int numTokens)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
    
    int pid = fork();   

    switch (pid)
    {
    case -1:
        printf("Error: Unable to fork process\n");
        break;

    case 0:
    {
        if(strcmp(args[numTokens-1], "&") == 0) args[--numTokens] = NULL;

        handleRedirection(args, numTokens);

        execvp(args[0], args);
        perror("exec");
        exit(1);
        break;
    }
    
    default:
        if(strcmp(args[numTokens-1], "&") != 0) waitpid(pid, NULL, 0);
    }
    return 0;
}

void handleRedirection(char* args[], int numTokens)
{
    for(int i = 0; i < numTokens-1; i++)
    {
        int channel = -1;
        if(strcmp(args[i], "<") == 0)      channel = 0;
        else if(strcmp(args[i], ">") == 0) channel = 1;

        if(channel != -1)
        {
            args[i] = NULL;
            char* file = args[i+1];
            int mode = channel == 0 ? O_RDONLY : O_WRONLY;

            close(channel);
            if(open(file, mode|O_CREAT, 0660) == -1)
            {
                perror("open");
                exit(1);
            }
        }
    }
}