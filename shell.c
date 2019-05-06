#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int f;

void create_args(char *buffer, char** args){
    char *token = strtok(buffer, " ");
    int i = 0;
    
    while(token != NULL){
        if(strcmp(token, ">") == 0){
           close(1);
           token = strtok(NULL, " ");
           f = open(token, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
           break;
        }
        args[i] = token;
        token = strtok(NULL, " ");

        i++;
    }
    args[i] = NULL;
}

int main(){
    char *args[500];
    int stdout_copy = dup(1);

    
    while(1){
        printf("shell$ ");
        
        char buffer[100];
        fgets(buffer, 100, stdin);
        
        if(buffer[strlen(buffer) - 1] == '\n'){
            buffer[strlen(buffer) - 1] = '\0';
        }
        
        create_args(buffer, args);
        
        if(strcmp(args[0], "exit") == 0){
            break;
        }else if(strcmp(args[0], "pwd") == 0){
            char temp[60];
            printf("%s\n", getcwd(temp,60));
        }else if(strcmp(args[0], "cd") == 0){
            char path[100];
            
            char temp[80];
            getcwd(temp,80);

            strcpy(path, temp);
            strcat(path, "/");
            strcat(path, args[1]);
                
            chdir(path);
        }else{
            int pid = fork();
            int i;
            
            if(pid == 0){
                execvp(args[0],args);
                
                perror("execvp");
                return -1;
            }else{
                wait();
            }
        }
        dup2(stdout_copy, 1);
    }
    return 0;
}