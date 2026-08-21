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
        print(STDERR_FILENO, "Usage: ./Solution <filen_name>\n");
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
        print(STDERR_FILENO, "Error failed to create second pipe!\n");
        exit(EXIT_FAILURE);
    }

    int child1 = fork();
    if(child1 == -1){
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[0]);
        print(STDERR_FILENO, "Error: Failed to create first child!");
        exit(EXIT_FAILURE);
    }
    if(child1 == 0){
        close(pipe1[0]);
        close(pipe2[0]); close(pipe2[1]);

        if(dup2(pipe1[1], STDOUT_FILENO) == -1){
            close(pipe1[1]);
            print(STDERR_FILENO, "Error: Failed to duplicate fd in first child!\n");
            exit(EXIT_FAILURE);
        }

        close(pipe1[1]);

        int fd = open(argv[1], O_RDONLY);
        if(fd == -1){
            print(STDERR_FILENO, "Error: Failed to open input file!");
            exit(EXIT_FAILURE);
        }

        int buff[4096];
        int bytes_read;
        while((bytes_read = read(fd, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                close(fd);
                print(STDERR_FILENO, "Error: Failed while writing in first child!\n");
                exit(EXIT_FAILURE);
            }
        }

        close(fd);

        if(bytes_read == -1){
            print(STDERR_FILENO, "Error: Failed while reading from file!\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    int child2 = fork();
    if(child2 == -1){
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        print(STDERR_FILENO, "Error: Failed to create second child!\n");
        exit(EXIT_FAILURE);
    }
    if(child2 == 0){
        close(pipe1[1]);
        close(pipe2[0]);

        if(dup2(pipe1[0], STDIN_FILENO) == -1){
            close(pipe1[0]);
            close(pipe2[1]);
            print(STDERR_FILENO, "Error: Failed to duplicate fd in second child!\n");
            exit(EXIT_FAILURE);
        }

        close(pipe1[0]);

        if(dup2(pipe2[1], STDOUT_FILENO) == -1){
            close(pipe2[1]);
            print(STDERR_FILENO, "Error: Failed to duplicate fd in second child!\n");
            exit(EXIT_FAILURE);
        }

        close(pipe2[1]);

        int buff[4096];
        int bytes_read;
        while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                print(STDERR_FILENO, "Error: Failed while writing in second child!\n");
                exit(EXIT_FAILURE);
            }
        }
        if(bytes_read == -1){
            print(STDERR_FILENO, "Error: Failed while reading in second child!\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    int child3 = fork();
    if(child3 == -1){
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        print(STDERR_FILENO, "Error: Failed to create third child!\n");
        exit(EXIT_FAILURE);
    }
    if(child3 == 0){
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[1]);
        
        if(dup2(pipe2[0], STDIN_FILENO) == -1){
            close(pipe2[0]);
            print(STDERR_FILENO, "Error: Failed to duplicate fd in third child!\n");
            exit(EXIT_FAILURE);
        }

        close(pipe2[0]);

        int buff[4096];
        int bytes_read;
        while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                print(STDERR_FILENO, "Error: Failed while writing in third child!\n");
                exit(EXIT_FAILURE);
            }
        }
        if(bytes_read == -1){
            print(STDERR_FILENO, "Error: Failed while reading in third child!\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    close(pipe1[0]); close(pipe1[1]);
    close(pipe2[0]); close(pipe2[1]);

    int status1, status2, status3;
    waitpid(child1, &status1, 0);
    waitpid(child2, &status2, 0);
    waitpid(child3, &status3, 0);

    return 0;
}