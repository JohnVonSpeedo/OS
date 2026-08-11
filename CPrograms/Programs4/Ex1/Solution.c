#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void write_err(char* err){
    int len = 0;
    while(err[len] != '\0')
        len++;
    write(STDERR_FILENO, err, len);
}

int main(int argc, char* argv[]){
    if(argc != 1){
        write_err("Error: Program takes no arguments!\n");
        exit(1);
    }

    int fd_out = open("abc", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if(fd_out == -1){
        write_err("Error: Failed to open or create file!\n");
        exit(1);
    }

    int child = fork();

    if(child == -1){
        write_err("Error: Failed to create child process!\n");
        close(fd_out);
        exit(1);
    }

    if(child == 0){
        if(write(fd_out, "hello\n", 6) != 6){
            write_err("Error: Child error while writing to file!\n");
            close(fd_out);
            exit(1);
        }
    }
    else{
        if(write(fd_out, "hello\n", 6) != 6){
            write_err("Error: Parent error while writing to file!\n");
            close(fd_out);
            exit(1);
        }

        int status;
        waitpid(child, &status, 0);
    }

    close(fd_out);
    return 0;
}