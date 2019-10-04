int running;

void executeCommand(char* args[], int numTokens);
int internalCmd(char* args[], int numTokens);
int externalCmd(char* args[], int numTokens);
void handleRedirection(char* args[], int numTokens);