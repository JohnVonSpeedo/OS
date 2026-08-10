#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void write_str(char* str){
    int len = 0;
    while(str[len] != '\0')
        len++;
    write(STDOUT_FILENO, str, len);
}

void write_err(char* err){
    int len = 0;
    while(err[len] != '\0')
        len++;
    write(STDERR_FILENO, err, len);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        write_err("Error: Program takes 2 commands as arguments!\n");
        exit(1);
    }

    int child1 = fork();

    if(child1 == -1){
        write_err("Error: Failed to create child process 1!\n");
        exit(1);
    }
    
    if(child1 == 0){
        execlp(argv[1], argv[1], NULL);
        write_err("Error: First command failed to execute first command or wasn't found!\n");
        exit(1);
    }
    else{
        int status1;

        waitpid(child1, &status1, 0);

        if(WIFEXITED(status1) && WEXITSTATUS(status1) == 0){
            int child2 = fork();

            if(child2 == -1){
                write_err("Error: Failed to create second child!\n");
                exit(1);
            }

            if(child2 == 0){
                execlp(argv[2], argv[2], NULL);
                write_err("Error: Failed to execute second command or wasn't found!\n");
                exit(1);
            }
            else{
                int status2;

                waitpid(child2, &status2, 0);
            }
        }
    }
    return 0;
}