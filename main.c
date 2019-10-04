#include <stdio.h>
#include <string.h>

#include "shell.h"

#define MAXC 2048
#define MAX_ARGS 50

char cmd[MAXC];
char *args[MAX_ARGS];

int makeargs(char *s, char *args[]);

int main()
{
    running = 1;
    while(running)
    {
        printf("sish> ");
        fgets(cmd, MAXC, stdin);
        int numTokens = makeargs(cmd, args);

        executeCommand(args, numTokens);
    }
    return 0;
}

int makeargs(char *s, char *args[])
{
    // in: s points a text string with words
    // pre: args is predefined as char *args[argsMAX]
    // out: args[] points to all words in the string s (*s is modified!)
    // return: number of words pointed to by the elements in args (or -1 in case of error)

    int ntokens;
    if (s == NULL || args == NULL || MAX_ARGS == 0) return -1;

    ntokens = 0;
    args[ntokens] = strtok(s, " \t\n");
    while ( (args[ntokens] !=  NULL) && (ntokens<MAX_ARGS) ) 
    {
        ntokens++;
        args[ntokens] = strtok(NULL, " \t\n"); // breaks 's' inline at separators
    }

    args[ntokens] = NULL; // terminate with NULL reference
    return ntokens;
}