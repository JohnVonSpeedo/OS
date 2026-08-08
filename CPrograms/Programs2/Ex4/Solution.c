#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    char buff[4096];
    ssize_t bytes_read;
    
    if(argc == 1){
        while((bytes_read = read(STDIN_FILENO, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                const char err_msg[] = "Error while writing to standard output!\n";
                write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
                exit(1);
            }
        }

        if(bytes_read == -1){
            const char err_msg[] = "Error while reading from standard input!\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
            exit(1);
        }
        return 0;
    }

    for(int i = 1; i < argc; i++){
        int fd_in = open(argv[i], O_RDONLY);
        if(fd_in == -1){
            const char err_msg[] = "Error while opening input file!\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
            exit(1);
        }

        while((bytes_read = read(fd_in, buff, sizeof(buff))) > 0){
            if(write(STDOUT_FILENO, buff, bytes_read) != bytes_read){
                const char err_msg[] = "Error while writing to standard output!\n";
                write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
                close(fd_in);
                exit(1);
            }
        }

        if(bytes_read == -1){
            const char err_msg[] = "Error while reading from file!\n";
            write(STDERR_FILENO, err_msg, sizeof(err_msg) - 1);
            close(fd_in);
            exit(1);
        }

        close(fd_in);
    }

    return 0;
}