#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void print(int fd, char* str){
    int len = 0;
    while(str[len] != '\0')
        len++;
    if(write(fd, str, len) != len)
        exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){
    if(argc != 2){
        print(STDERR_FILENO, "Usage: <program> <file_name>\n");
        exit(EXIT_FAILURE);
    }

    int pipe1[2];
    if(pipe(pipe1) == -1){
        print(STDERR_FILENO, "Error: Failed to create pipe!\n");
        exit(EXIT_FAILURE);
    }

    int child1 = fork();
    if(child1 == -1){
        print(STDERR_FILENO, "Error: Failed to create first child!\n");
        close(pipe1[0]); close(pipe1[1]);
        exit(EXIT_FAILURE);
    }

    if(child1 == 0){
        close(pipe1[1]);

        if(dup2(pipe1[0], STDIN_FILENO) == -1){
            print(STDERR_FILENO, "Error: dup2() failed in child!\n");
            close(pipe1[0]); close(pipe1[1]);
            exit(EXIT_FAILURE);
        }

        close(pipe1[0]);

        int bytes_read;
        int buff[4096];
        while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                print(STDERR_FILENO, "Error: Failed while writing to STDOUT_FILENO!\n");
                exit(EXIT_FAILURE);
            }
        }
        if(bytes_read == -1){
            print(STDERR_FILENO, "Error: Failed while reading from parent!\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    int child2 = fork();
    if(child2 == -1){
        print(STDERR_FILENO, "Error: Failed to create second child!\n");
    }
    
    if(child2 == 0){
        close(pipe1[0]);

        int fd = open(argv[1], O_RDONLY);
        if(fd == -1){
            print(STDERR_FILENO, "Error: Failed to open file!\n");
            close(pipe1[1]);
            exit(EXIT_FAILURE);
        }

        if(dup2(pipe1[1], STDOUT_FILENO) == -1){
            print(STDERR_FILENO, "Error: Second dup2() failed in parent!\n");
            close(pipe1[1]);
            close(fd);
            exit(EXIT_FAILURE);
        }

        close(pipe1[1]);
        
        int bytes_read;
        int buff[4096];

        while((bytes_read = read(fd, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                print(STDERR_FILENO, "Error: Failed to write to STDOUT_FILENO!\n");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }

        close(fd);

        if(bytes_read == -1){
            print(STDERR_FILENO, "Error: Failed to read from file!\n");
            exit(EXIT_FAILURE);
        }

        print(STDOUT_FILENO, "\n");
        exit(EXIT_SUCCESS);
    }

    close(pipe1[0]); close(pipe1[1]);

    int status1, status2;
    waitpid(child1, &status1, 0);
    waitpid(child2, &status2, 0);

    return 0;
}