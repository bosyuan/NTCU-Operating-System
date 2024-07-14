#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_LINE_LENGTH 60
#define MAX_ARG_LENGTH 10

int main() {
    int simpleShell = 1;
    char cmd[MAX_LINE_LENGTH];
    pid_t pid;

    while (simpleShell) {
        fflush(stdin);
        printf("osh> ");
        char *args[4] = {};
        fgets(cmd, MAX_LINE_LENGTH, stdin);
        args[0] = (char *)malloc(sizeof(char)*MAX_ARG_LENGTH);
        int curr_arg = 0;
        int idx = 0;

        for (int i=0; i<strlen(cmd); i++) {
            if (cmd[i] == ' ') {
                curr_arg++;
                args[curr_arg] = (char *)malloc(sizeof(char)*MAX_ARG_LENGTH);
                idx = 0;
                continue;
            }
            else if (cmd[i] == '\n') break;
            *(args[curr_arg]+idx) = cmd[i]; 
            idx++;
        }
        
        if (!strcmp(args[0], "exit")){
            printf("Process end\n");
            break;
        } 

        curr_arg++;
        args[curr_arg] = NULL;
        int i=0;
        
        pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            exit(0);
        }
        else {
            wait(NULL);
            continue;
        }
        
    }

    return 0;
}