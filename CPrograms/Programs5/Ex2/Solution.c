#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print(int fd, char* str){
    int len = 0;
    while(str[len] != '\0')
        len++;
    if(write(fd, str, len) != len)
        exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){
    if(argc != 4){
        print(STDERR_FILENO, "Usage: <program> <cmd1> <cmd2> <cmd3>\n");
        exit(EXIT_FAILURE);
    }

    int pipe1[2];
    if(pipe(pipe1) == -1){
        print(STDERR_FILENO, "Error: Failed to create first pipe!\n");
        exit(EXIT_FAILURE);
    }

    int pipe2[2];
    if(pipe(pipe2) == -1){
        close(pipe1[0]); close(pipe1[1]);
        print(STDERR_FILENO, "Error: Failed to create second pipe!\n");
        exit(EXIT_FAILURE);
    }

    int child1 = fork();
    if(child1 == -1){
        print(STDERR_FILENO, "Error: Failed to create first child!\n");
        exit(EXIT_FAILURE);
    }

    if(child1 == 0){
        close(pipe1[0]);
        close(pipe2[0]); close(pipe2[1]);

        if(dup2(pipe1[1], STDOUT_FILENO) == -1){
            print(STDERR_FILENO, "Error: dup2() fail!");
            close(pipe1[1]);
            exit(EXIT_FAILURE);
        }

        close(pipe1[1]);

        execlp("sh", "sh", "-c", argv[1], NULL);
        print(STDERR_FILENO, "Error: Failed to execute first command!\n");
        exit(EXIT_FAILURE);
    }

    int child2 = fork();
    if(child2 == -1){
        print(STDERR_FILENO, "Error: Failed to create second child!\n");
        exit(EXIT_FAILURE);
    }

    if(child2 == 0){
        close(pipe1[1]);
        close(pipe2[0]);

        if(dup2(pipe1[0], STDIN_FILENO) == -1){
            print(STDERR_FILENO, "Error: dup2() fail!\n");
            close(pipe1[0]);
            close(pipe2[1]);
            exit(EXIT_FAILURE);
        }

        if(dup2(pipe2[1], STDOUT_FILENO) == -1){
            print(STDERR_FILENO, "Error: dup2() fail!\n");
            close(pipe1[0]);
            close(pipe2[1]);
            exit(EXIT_FAILURE);
        }

        close(pipe1[0]);
        close(pipe2[1]);

        execlp("sh", "sh", "-c", argv[2], NULL);
        print(STDERR_FILENO, "Error: Failed to execute second command!\n");
        exit(EXIT_FAILURE);
    }

    int child3 = fork();
    if(child3 == -1){
        print(STDERR_FILENO, "Failed to create third child!\n");
        exit(EXIT_FAILURE);
    }

    if(child3 == 0){
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[1]);

        if(dup2(pipe2[0], STDIN_FILENO) == -1){
            print(STDERR_FILENO, "Error: dup2() fail!\n");
            close(pipe2[0]);
            exit(EXIT_FAILURE);
        }

        close(pipe2[0]);

        execlp("sh", "sh", "-c", argv[3], NULL);
        print(STDERR_FILENO, "Error: Failed to execute third command!\n");
        exit(EXIT_FAILURE);
    }

    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);

    int status1, status2, status3;
    waitpid(child1, &status1, 0);
    waitpid(child2, &status2, 0);
    waitpid(child3, &status3, 0);

    return 0;
}