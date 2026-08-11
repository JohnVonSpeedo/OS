#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print(char* str, int fd){
    int len = 0;
    while(str[len] != '\0')
        len++;
    if(write(fd, str, len) != len)
        exit(1);
}

int main(int argc, char* argv[]){
    if(argc != 4){
        print("Error: Usage: <program> <cmd1> <cmd2>", STDERR_FILENO);
        exit(1);
    }

    int child1 = fork();

    if(child1 == -1){
        print("Error: Failed to create first child!\n", STDERR_FILENO);
        exit(1);
    }

    if(child1 == 0){
        execlp(argv[1], argv[1], NULL);

        print("Error: Failed to execute first command!\n", STDERR_FILENO);
        exit(1);
    }
    else{
        int status1;
        waitpid(child1, &status1, 0);

        if(WIFEXITED(status1) && WEXITSTATUS(status1) == 0){
            int fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(fd == -1){
                print("Error: Failed to open file!\n", STDERR_FILENO);
                exit(1);
            }

            print(argv[1], fd);
            print("\n", fd);
            close(fd);
        }
        else
            print("Eror: First command failed to terminate!\n", STDERR_FILENO);
    }

    int child2 = fork();

    if(child2 == -1){
        print("Error: Failed to create first child!\n", STDERR_FILENO);
        exit(1);
    }

    if(child2 == 0){
        execlp(argv[2], argv[2], NULL);

        print("Error: Failed to execute second command!\n", STDERR_FILENO);
        exit(1);
    }
    else{
        int status2;
        waitpid(child2, &status2, 0);
    }
    return 0;
}